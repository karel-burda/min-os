#include "video.h"

// original values:width=80, height=25
const size_t C_VGA_WIDTH = 80;
const size_t C_VGA_HEIGHT = 25;

uint8_t video_make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | (bg << 4);
}

uint16_t video_make_vga_entry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;

	return c16 | (color16 << 8);
}
