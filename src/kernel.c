#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_scroll_up(void)
{
    const size_t MAX_INDEX = (VGA_HEIGHT-1) * VGA_WIDTH + VGA_WIDTH-1;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			const size_t next_row_index = (y+1) * VGA_WIDTH + x;
			terminal_buffer[index] = next_row_index < MAX_INDEX ? terminal_buffer[next_row_index] : vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
    // Put the character
    if (c == '\n'){
        // Set it to VGA_WIDTH-1 so then it goes down with the increment
		terminal_column = VGA_WIDTH-1;
    } else{
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    }

    // Handle column and row increment
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT){
            terminal_scroll_up();
            terminal_row = VGA_HEIGHT-1;
        }
    }
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void kernel_main(void)
{
	/* Initialize terminal interface */
	terminal_initialize();

	terminal_writestring("00: Hello, kernel World!\n");
	terminal_writestring("01: Hello again, kernel World!\n");
	terminal_writestring("02: Hello again, kernel World!\n");
	terminal_writestring("03: Hello again, kernel World!\n");
	terminal_writestring("04: Hello again, kernel World!\n");
	terminal_writestring("05: Hello again, kernel World!\n");
	terminal_writestring("06: Hello again, kernel World!\n");
	terminal_writestring("07: Hello again, kernel World!\n");
	terminal_writestring("08: Hello again, kernel World!\n");
	terminal_writestring("09: Hello again, kernel World!\n");

	terminal_writestring("10: Hello again, kernel World!\n");
	terminal_writestring("11: Hello again, kernel World!\n");
	terminal_writestring("12: Hello again, kernel World!\n");
	terminal_writestring("13: Hello again, kernel World!\n");
	terminal_writestring("14: Hello again, kernel World!\n");
	terminal_writestring("15: Hello again, kernel World!\n");
	terminal_writestring("16: Hello again, kernel World!\n");
	terminal_writestring("17: Hello again, kernel World!\n");
	terminal_writestring("18: Hello again, kernel World!\n");
	terminal_writestring("19: Hello again, kernel World!\n");

	terminal_writestring("20: Hello again, kernel World!\n");
	terminal_writestring("21: Hello again, kernel World!\n");
	terminal_writestring("22: Hello again, kernel World!\n");
	terminal_writestring("23: Hello again, kernel World!\n");
	terminal_writestring("24: Hello again, kernel World!\n");
	terminal_writestring("25: Hello again, kernel World!\n");
	terminal_writestring("26: Hello again, kernel World!\n");
	terminal_writestring("27: Hello again, kernel World!\n");
	terminal_writestring("28: Hello again, kernel World!\n");
	terminal_writestring("29: Hello again, kernel World!\n");

	terminal_writestring("30: Hello again, kernel World!\n");
	terminal_writestring("31: Hello again, kernel World!\n");
	terminal_writestring("32: Hello again, kernel World!\n");
	terminal_writestring("33: Hello again, kernel World!\n");
	terminal_writestring("34: Hello again, kernel World!\n");
	terminal_writestring("35: Hello again, kernel World!\n");
	terminal_writestring("36: Hello again, kernel World!\n");
	terminal_writestring("37: Hello again, kernel World!\n");
	terminal_writestring("38: Hello again, kernel World!\n");
	terminal_writestring("39: Hello again, kernel World!\n");
}
