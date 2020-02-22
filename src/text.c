#include <gb/font.h>
#include "text.h"

void init_text() {
  font_init();
  font_load(font_min);
}

void printc_win(UBYTE x, UBYTE y, char c) {
  const font_map[] = {
    // 0 - 9
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,

    // A - Z
    0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25,

    // ' '
    0x00,
  };

  if (c >= 48 && c <= 57) {
    set_win_tiles(x, y, 1, 1, font_map + c - 48);
  } else if (c >= 65 && c <= 90) {
    set_win_tiles(x, y, 1, 1, font_map + c - 55);
  } else if (c >= 97 && c <= 122) {
    set_win_tiles(x, y, 1, 1, font_map + (c - 87));
  } else {
    set_win_tiles(x, y, 1, 1, font_map + 36);
  }
}

void prints_win(UBYTE x, UBYTE y, char *c) {
  UBYTE i = 0;
  while (*c != '\0') {
    printc_win(x+i, y, *c);
    c++;
    i++;
  }
}

void printi_win(UBYTE x, UBYTE y, UWORD i) {
  int j = 4;
  char c[6] = { ' ', ' ', ' ', ' ', ' ', '\0' };

  if (i == 0) {
    c[j] = '0';
  }

  while (i > 0 && j < 5) {
    c[j] = (i % 10) + '0';
    i /= 10;
    j--;
  }

  prints_win(x, y, c);
}
