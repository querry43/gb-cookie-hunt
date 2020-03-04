#include <gb/gb.h>

const unsigned char white[] = {
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
};

const unsigned char light_gray[] = {
  0xff, 0x00,
  0xff, 0x00,
  0xff, 0x00,
  0xff, 0x00,
  0xff, 0x00,
  0xff, 0x00,
  0xff, 0x00,
  0xff, 0x00,
};

const unsigned char dark_gray[] = {
  0x00, 0xff,
  0x00, 0xff,
  0x00, 0xff,
  0x00, 0xff,
  0x00, 0xff,
  0x00, 0xff,
  0x00, 0xff,
  0x00, 0xff,
};

const unsigned char black[] = {
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
};

const unsigned char map[] = {
  0x00, 0x01, 0x02, 0x03,
};

void main()
{
  BYTE x, y;
  BYTE j;

  disable_interrupts();
  DISPLAY_OFF;

  set_bkg_data(0, 1, white);
  set_bkg_data(1, 1, light_gray);
  set_bkg_data(2, 1, dark_gray);
  set_bkg_data(3, 1, black);

  for (y = 0; y < 18; y++)
    for (x = 0; x + y < 20; x += 5)
      set_bkg_tiles(x + y, y, 4, 1, map);

  HIDE_SPRITES;
  HIDE_WIN;
  SHOW_BKG;

  DISPLAY_ON;

  while (TRUE) {
    delay(10);
    j = waitpad(J_UP | J_DOWN | J_LEFT | J_RIGHT);

    if (j & J_UP) {
      scroll_bkg(0, -1);
    } else if (j & J_DOWN) {
      scroll_bkg(0, 1);
    }

    if (j & J_LEFT) {
      scroll_bkg(-1, 0);
    } else if (j & J_RIGHT) {
      scroll_bkg(1, 0);
    }
  }
}

// vim: ts=2 sw=2

