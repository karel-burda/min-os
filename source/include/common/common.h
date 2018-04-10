#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

size_t strlen(const char * str);
char char_to_upper (const char c);

void log (const char * str);

void sleep (size_t ticks);

#endif
