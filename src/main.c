#include <gb/gb.h>
#include <stdio.h>
#include "../tile-data/tileset.h"

#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define MAX(x,y) ((x) < (y) ? (y) : (x))

void init();

void main()
{
  init();

  while(TRUE) {
    char j = joypad();
    delay(5);

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

void init() {
  int x, y;

  disable_interrupts();
  DISPLAY_OFF;

  set_win_data(0, 26, tileset);

  for (x = 0; x < 32; x += 4)
    for (y = 0; y < 32; y += 4)
      set_bkg_tiles(x, y, 4, 4, background_map);

  set_bkg_tiles(2, 0, 2, 2, cookie_map);
  set_bkg_tiles(16, 6, 2, 2, cookie_map);
  set_bkg_tiles(13, 11, 2, 2, cookie_map);

  set_win_tiles(0, 0, 20, 2, hud_map);

  move_win(7, 128);

  SHOW_BKG;
  SHOW_WIN;
  DISPLAY_ON;
  enable_interrupts();
}

// vim: ts=2 sw=2
