#ifndef VIDEO_H
#define VIDEO_H

#include <stddef.h>
#include <stdint.h>

/* Hardware text mode color constants. */
enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

extern const size_t C_VGA_WIDTH;
extern const size_t C_VGA_HEIGHT;

uint8_t video_make_color(enum vga_color fg, enum vga_color bg);
uint16_t video_make_vga_entry(char c, uint8_t color);

#endif
