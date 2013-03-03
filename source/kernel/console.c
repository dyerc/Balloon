#include <kernel.h>

uint16_t *videomem = (uint16_t *)0xB8000;

uint32_t xPos = 0;
uint32_t yPos = 0;

uint8_t backColour = 0;
uint8_t foreColour = 15;

void console_scroll()
{
  uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
  uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

  // Row 25 is the end, this means we need to scroll up
  if(yPos >= 25)
  {
    int i;

    for (i = 0 * 80; i < 24 * 80; i++)
      videomem[i] = videomem[i + 80];

    for (i = 24 * 80; i < 25 * 80; i++)
      videomem[i] = blank;

    yPos = 24;
  }
}

void console_move_cursor()
{
  uint16_t position = yPos * 80 + xPos;
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((position >> 8)&0xFF));
  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(position & 0xFF));
}

void console_clear()
{
  uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
  uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

  int i;
  for (i = 0; i < 80 * 25; i++)
    videomem[i] = blank;

  // Move the hardware cursor back to the start.

  xPos = 0;
  yPos = 0;

  console_move_cursor();
}

void console_putchar(char c)
{
  uint8_t  attributeByte = (backColour << 4) | (foreColour & 0x0F);
  uint16_t attribute = attributeByte << 8;
  uint16_t *location;

  if (c == 0x08 && xPos > 0)
  {
    // Backspace
    xPos--;
  }
  else if (c == 0x09)
  {
    // Tab
    xPos = (xPos + 8) & ~(8 - 1);
  }
  else if (c == '\r')
  {
    xPos = 0;
  }
  else if (c == '\n')
  {
    yPos++;
    xPos = 0;
  }
  else if (c >= ' ')
  {
    // Other printable character
    location = videomem + (yPos * 80 + xPos);
    *location = c | attribute;

    xPos++;
  }

  console_scroll();
  console_move_cursor();
}

void console_puts(char *txt)
{
  uint32_t i = 0;

  while(txt[i])
  {
    console_putchar(txt[i]);
    i++;
  }
}

void init_console()
{
  console_clear();
}
