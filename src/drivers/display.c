
#include "ports.h"
#include "display.h"
#include "utils.h"

int cursor_x = 0, cursor_y = 0;  // cursor location

// The attribute byte is made up of two nibbles - the lower being the
// foreground colour, and the upper the background colour.
int attrib = 0x0F;  // foreground and background colors

u16int *video_memory = (u16int *) VIDEO_MEMORY_ADDRESS;


// Sets the forecolor and backcolor
void settextcolor(u16int forecolor, u16int backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void update_cursor(int x, int y)
{
	u16int pos = y * SCREEN_COLUMNS + x;


	outb(0x3D4, 14);  // Tell the VGA board we are setting the high cursor byte.
	outb(0x3D5, (u8int) (pos & 0xFF));  // Send the high cursor byte.
	outb(0x3D4, 15);  // Tell the VGA board we are setting the low cursor byte.
	outb(0x3D5, (u8int) ((pos >> 8) & 0xFF));  // Send the low cursor byte.
  cursor_x = x;
  cursor_y = y;
}

// clear the screen, write spaces to the screen's framebuffer
void clear()
{

	u16int blank = 0x20 | (attrib << 8);

	int i;
	for (i = 0; i < SCREEN_ROWS * SCREEN_COLUMNS; i++)
	{
		video_memory[i] = blank;
	}

	// Move the hardware cursor back to the start.
	update_cursor(0, 0);
}

// Scrolls the text on the screen up by one line.
void scroll()
{
	// Get a space character with the default colour attributes.
	u16int blank = 0x20 | (attrib << 8);

	// Row 25 is the end, this means we need to scroll up
	if(cursor_y >= SCREEN_ROWS)
	{
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		int i;
		for (i = 0; i < (SCREEN_ROWS - 1) * SCREEN_COLUMNS; i++)
		{
			video_memory[i] = video_memory[i + SCREEN_COLUMNS];
		}

		// The last line should now be blank. Do this by writing
		// 80 spaces to it.
		for (i = (SCREEN_ROWS - 1) * SCREEN_COLUMNS; i < SCREEN_ROWS * SCREEN_COLUMNS; i++)
		{
			video_memory[i] = blank;
		}
		// The cursor should now be on the last line.
		cursor_y = SCREEN_ROWS - 1;
	}
}

// Print a single char to specific location in the screen.
void putc(char c)
{


	// The attribute byte is the top 8 bits of the word we have to send to the
	// VGA board.
	u16int attribute = attrib  << 8;
  u16int *location;

	// Handle a backspace, by moving the cursor back one space
	if (c == 0x08 && cursor_x)
	{
		cursor_x--;
	}

	// Handle a tab by increasing the cursor's X, but only to a point
	// where it is divisible by 8.
	else if (c == 0x09)
	{
		cursor_x = (cursor_x+8) & ~(8-1);
	}

	// Handle carriage return
	else if (c == '\r')
	{
		cursor_x = 0;
	}

	// Handle newline by moving cursor back to left and increasing the row
	else if (c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}
	// Handle any other printable character.
	else if(c >= ' ')
	{
		location = video_memory + (cursor_y * SCREEN_COLUMNS + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}

	// Check if we need to insert a new line because we have reached the end
	// of the screen.
	if (cursor_x >= SCREEN_COLUMNS)
	{
		cursor_x = 0;
		cursor_y ++;
	}

	// Scroll the screen if needed.
	scroll();
	// Move the hardware cursor.
	update_cursor(cursor_x, cursor_y);
}

// Print a string to the screen
void kprint(char *c)
{
	u8int temp_x = cursor_x;
	u8int temp_y = cursor_y;

	int i = 0;
	while (c[i])
	{
		putc(c[i++]);
	}
}
