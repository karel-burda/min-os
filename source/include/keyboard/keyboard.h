#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stddef.h>

bool kbd_init();
void kbd_start_read();
void keyboard_disable();
void keyboard_enable();

#endif
