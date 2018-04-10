#include "terminal.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "chars.h"
#include "common.h"
#include "io.h"
#include "keyboard.h"
#include "programs/logo/logo.h"
#include "shell.h"
#include "strings.h"
#include "terminal_test.h"
#include "video.h"

static void terminal_update_cursor(int row, int col);
static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
static void terminal_handle_next_char();
static size_t terminal_get_idx (int row, int col);

static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = 0x0;
static uint16_t * terminal_buffer = 0x0;
static long terminal_last_editable_index = -1;
static size_t terminal_temp_buff_idx = 0;

bool terminal_init()
{
    bool ret = true;

    terminal_row = 0;
    terminal_column = 0;
    terminal_color = video_make_color(COLOR_WHITE, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;

    for (size_t y = 0; y < C_VGA_HEIGHT; ++y)
    {
        for (size_t x = 0; x < C_VGA_WIDTH; ++x)
        {
            const size_t index = y * C_VGA_WIDTH + x;
            terminal_buffer[index] = video_make_vga_entry(' ', terminal_color);
        }
    }

    terminal_clean_temp_buff();

    // init keyboard
    ret &= kbd_init();

    // run test
    ret &= terminal_self_test();

    return ret;
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = video_make_color(color, COLOR_BLACK);
}

void terminal_putchar(char c, bool update_cursor)
{
    // newline
    if (c == CHAR_NEWLINE)
    {
        terminal_row++;
        terminal_column = 0;
    }
    // backspace
    else if (c == CHAR_BACKPACE)
    {
        // is it possible to delete the last char?
        const size_t idx_new = terminal_get_idx(terminal_row, terminal_column-1);
        if (idx_new < terminal_last_editable_index)
        {
            return;
        }

        // TODO: handle other lines when reached
        terminal_column--;
        terminal_putentryat(CHAR_SPACE, terminal_color, terminal_column, terminal_row);
    }
    // tab
    else if (c == CHAR_TAB)
    {
        // TODO: handle newlines
        for (unsigned char i=0; i<3; i++)
        {
            terminal_putentryat(CHAR_SPACE, terminal_color, terminal_column, terminal_row);

            terminal_handle_next_char();
        }
    }
    // other
    else
    {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

        terminal_handle_next_char();
    }

    if (update_cursor)
    {
        terminal_update_cursor(terminal_row, terminal_column);
    }
}

void terminal_writestring(const char * data)
{
    size_t datalen = strlen(data);

    for (size_t i = 0; i < datalen; i++)
    {
        terminal_putchar(data[i], false);
    }
}

bool terminal_run()
{
    // write logo
    logo_render();

    terminal_writestring(C_STR_KERNEL_WELCOME);
    terminal_writestring(C_STR_START_TYPING);

    return shell_init();
}

void terminal_mark_last_editable_idx()
{
    terminal_last_editable_index = (terminal_row*C_VGA_WIDTH) + terminal_column;
}

void terminal_clean_temp_buff()
{
    for (size_t i = 0; i < sizeof(terminal_temp_buff)/sizeof(terminal_temp_buff[0]); ++i)
    {
        terminal_temp_buff[i] = CHAR_SPACE;
    }

    terminal_temp_buff_idx = 0;
}

void terminal_end_temp_buff()
{
    terminal_temp_buff[terminal_temp_buff_idx] = 0;
}

static void terminal_update_cursor(int row, int col)
 {
    const size_t position = terminal_get_idx(row, col);

    // cursor LOW port to vga INDEX register
    outportb(0x3D4, 0x0F);
    outportb(0x3D5, (unsigned char)(position & 0xFF));
    // cursor HIGH port to vga INDEX register
    outportb(0x3D4, 0x0E);
    outportb(0x3D5, (unsigned char )((position >> 8) & 0xFF));
 }

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = terminal_get_idx(y, x);

    terminal_buffer[index] = video_make_vga_entry(c, color);

    // TODO: fix
    terminal_temp_buff_idx = x;
    terminal_temp_buff[terminal_temp_buff_idx++] = c;
}

static void terminal_handle_next_char()
{
    if ((++terminal_column) == C_VGA_WIDTH)
    {
        terminal_column = 0;

        if ((++terminal_row) == C_VGA_HEIGHT)
        {
            terminal_row = 0;
        }
    }
}

static size_t terminal_get_idx (int row, int col)
{
    return (row*C_VGA_WIDTH) + col;
}
