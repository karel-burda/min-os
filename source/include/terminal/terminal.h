#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool terminal_init();
void terminal_setcolor(uint8_t color);
void terminal_putchar(char c, bool update_cursor);
void terminal_writestring(const char * data);
bool terminal_run();
void terminal_mark_last_editable_idx();

char terminal_temp_buff[1024];

void terminal_clean_temp_buff();
void terminal_end_temp_buff();

#endif
