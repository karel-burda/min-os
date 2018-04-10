#include "common.h"

#include <stddef.h>

#include "terminal.h"

size_t strlen(const char * str)
{
	if (str == 0)
	{
		return 0;
	}

	size_t count = 0;

	while (str[count] != 0)
	{
		count++;
    }

	return count;
}

void log (const char * str)
{
	terminal_writestring("\nLOG: '");
	terminal_writestring(str);
	terminal_writestring("'\n");
}

char char_to_upper (const char c)
{
	/* based on the ASCII */
	if (c >= 'a' && c <= 'z')
	{
		return c-32;
	}

	return c;
}

void sleep (size_t ticks)
{
    size_t i = 0;

    while (i++ <= ticks);
}
