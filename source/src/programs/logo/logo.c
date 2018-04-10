#include "programs/logo/logo.h"

#include "terminal.h"
#include "video.h"

void logo_render()
{
    terminal_setcolor(COLOR_GREEN);
    terminal_writestring("******************");
    terminal_writestring("\n");
    terminal_writestring("******************");
    terminal_writestring("\n");
    terminal_writestring("*     ");
    terminal_setcolor(COLOR_RED);
    terminal_writestring("MIN-OS");
    terminal_setcolor(COLOR_GREEN);
    terminal_writestring("     *");
    terminal_writestring("\n");
    terminal_writestring("******************");
    terminal_writestring("\n");
    terminal_writestring("******************");
    terminal_writestring("\n");
    terminal_setcolor(COLOR_WHITE);
}
