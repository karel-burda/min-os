#include "shell.h"

#include <stdbool.h>

#include "chars.h"
#include "common.h"
#include "keyboard.h"
#include "strings.h"
#include "std.h"
#include "terminal.h"

bool shell_init()
{
    log("shell_init()");

    terminal_putchar(CHAR_SHELL_INIT, true);

    terminal_clean_temp_buff();

    kbd_start_read();

    return true;
}

void shell_interpret(const char * str)
{
    terminal_putchar(CHAR_NEWLINE, false);
    terminal_writestring(str);
    terminal_putchar(CHAR_NEWLINE, false);

    terminal_putchar(CHAR_SHELL_INIT, true);

    terminal_mark_last_editable_idx();
    terminal_clean_temp_buff();
}
