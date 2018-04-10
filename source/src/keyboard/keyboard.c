#include "keyboard.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "io.h"
#include "keyboard_layout.h"
#include "terminal.h"
#include "shell.h"
#include "std.h"

static void keyboard_resolve_enter();
static void keyboard_resolve_backspace();
static uint8_t kybrd_ctrl_read_status();
static uint8_t kybrd_enc_read_buf();
static void kybrd_ctrl_send_cmd(uint8_t cmd);
static void kybrd_enc_send_cmd(uint8_t cmd);
static unsigned char kbd_resolve_pressed_code(unsigned char code, bool shift_pressed);
static void kbd_acknowledge_scan_code();

// required num of ticks after the system decises that the key is being pressed for long enough to repeat
static const size_t C_KEY_PRESSED_REQ_TICKS = 200000;
static bool kbd_is_enabled = false;
static bool kbd_shift_pressed = false;

enum KYBRD_ENCODER_IO
{
    KYBRD_ENC_INPUT_BUF = 0x60,
    KYBRD_ENC_CMD_REG = 0x60
};

enum KYBRD_CTRL_IO
{
    KYBRD_CTRL_STATS_REG = 0x64,
    KYBRD_CTRL_CMD_REG = 0x64
};

enum KYBRD_ENC_CMDS
{
    KYBRD_ENC_CMD_SET_LED = 0xED,
    KYBRD_ENC_CMD_RESET = 0xFF
};

enum KYBRD_CTRL_CMDS
{
    KYBRD_CTRL_CMD_DISABLE =    0xAD,
    KYBRD_CTRL_CMD_ENABLE = 0xAE
};

enum KYBRD_CTRL_STATS_MASK
{
    KYBRD_CTRL_STATS_MASK_OUT_BUF   =   1,
    KYBRD_CTRL_STATS_MASK_IN_BUF    =   2,
    KYBRD_CTRL_STATS_MASK_SYSTEM    =   4,
    KYBRD_CTRL_STATS_MASK_CMD_DATA  =   8,
    KYBRD_CTRL_STATS_MASK_LOCKED    =   0x10,
    KYBRD_CTRL_STATS_MASK_AUX_BUF   =   0x20,
    KYBRD_CTRL_STATS_MASK_TIMEOUT   =   0x40,
    KYBRD_CTRL_STATS_MASK_PARITY    =   0x80
};

// sets leds
void kbd_set_leds (bool num, bool caps, bool scroll)
{
    uint8_t data = 0;

    // set or clear the bit
    data = (scroll) ? (data | 1) : (data & 1);
    data = (num) ? (num | 2) : (num & 2);
    data = (caps) ? (num | 4) : (num & 4);

    // send the command -- update keyboard Light Emetting Diods (LEDs)
    kybrd_enc_send_cmd (KYBRD_ENC_CMD_SET_LED);
    kybrd_enc_send_cmd (data);
}

// disables the keyboard
void keyboard_disable()
{
    kybrd_ctrl_send_cmd (KYBRD_CTRL_CMD_DISABLE);
    kbd_is_enabled = false;
}

// enables the keyboard
void keyboard_enable()
{
    kybrd_ctrl_send_cmd (KYBRD_CTRL_CMD_ENABLE);
    kbd_is_enabled = true;
}

bool kbd_init()
{
    log("keyboard init");
    keyboard_enable();
    kbd_set_leds(true, false, false);

    return true;
}

// TODO: handle delete, arrows, etc.
void kbd_start_read()
{
    unsigned char code = 0;
    unsigned char last_code = code;
    kbd_shift_pressed = false;
    bool repeat_char = false;
    // num of ticks when certain key is being hold
    size_t key_pressed_ticks = 0;

    while (true)
    {
        code = inportb(KYBRD_ENC_INPUT_BUF);

        kbd_acknowledge_scan_code();

        // key has just been released
        if (code & KYBRD_CTRL_STATS_MASK_PARITY)
        {
            if((code ^ KYBRD_CTRL_STATS_MASK_PARITY) == C_IDX_LSHIFT)
            {
                kbd_shift_pressed = false;
            }
        }
        // key has been pressed right now
        else
        {
            // resolve shift
            if (code == C_IDX_LSHIFT)
            {
                kbd_shift_pressed = true;

                continue;
            }

            // another key has been pressed
            if (code != last_code)
            {
                if (code == C_IDX_ENTER)
                {
                    keyboard_resolve_enter();
                }
                else
                {
                    terminal_putchar(kbd_resolve_pressed_code(code, kbd_shift_pressed), true);
                }

                key_pressed_ticks = 0;
            }

            // the same key was pressed (in the last ticks)
            // inspect the difference between the last press of that key
            else
            {
                repeat_char = false;

                // backspace is being carried out faster
                if (code == C_IDX_BACKSPACE)
                {
                    if (key_pressed_ticks++ >= (C_KEY_PRESSED_REQ_TICKS/2))
                    {
                        repeat_char = true;
                    }
                }
                else
                {
                    if (key_pressed_ticks++ >= C_KEY_PRESSED_REQ_TICKS)
                    {
                        repeat_char = true;
                    }
                }

                // key is being pressed for long enough
                if (repeat_char)
                {
                    if (code == C_IDX_ENTER)
                    {
                        keyboard_resolve_enter();
                    }
                    else
                    {
                        terminal_putchar(kbd_resolve_pressed_code(code, kbd_shift_pressed), true);
                    }

                    key_pressed_ticks = 0;
                }
            }
        }

        last_code = code;
    }
}

static void keyboard_resolve_enter()
{
    //shell_interpret(terminal_get_temp_buff());
    // TODO: fix
    terminal_end_temp_buff();
    shell_interpret(terminal_temp_buff);
    terminal_clean_temp_buff();
}

// read status from keyboard controller
static uint8_t kybrd_ctrl_read_status()
{
    return inportb (KYBRD_CTRL_STATS_REG);
}

// reads keyboard encoder buffer
static uint8_t kybrd_enc_read_buf()
{
    return inportb (KYBRD_ENC_INPUT_BUF);
}

// sends command byte to keyboard controller
static void kybrd_ctrl_send_cmd(uint8_t cmd)
{
    // wait for kkybrd controller input buffer to be clear
    while (1)
        if ( (kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
            break;

    outportb (KYBRD_CTRL_CMD_REG, cmd);
}

// sends command byte to keyboard encoder
static void kybrd_enc_send_cmd(uint8_t cmd)
{
    // wait for kkybrd controller input buffer to be clear
    while (1)
        if ((kybrd_ctrl_read_status () & KYBRD_CTRL_STATS_MASK_IN_BUF) == 0)
            break;

    // send command byte to kybrd encoder
    outportb (KYBRD_ENC_CMD_REG, cmd);
}

static unsigned char kbd_resolve_pressed_code(unsigned char code, bool shift_pressed)
{
    return shift_pressed ? kbd_us_shift[code] : kbd_us[code];
}

static void kbd_acknowledge_scan_code()
{
    // get keyboard control register
    register int i = inportb(0x61);

    // toggle acknowledge bit high
    outportb(0x61, i | KYBRD_CTRL_STATS_MASK_PARITY);

    // toggle acknowledge bit low
    outportb(0x61, i);

    // acknowledge the interrupt to the programmable interrupt controller
    outportb(0x20, 0x20);
}
