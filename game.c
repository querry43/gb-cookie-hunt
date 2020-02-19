#include <gb/gb.h>
#include <stdio.h>
#include "title.h"

#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define MAX(x,y) ((x) < (y) ? (y) : (x))

// 2BPP format
const UBYTE white[] = {
  0x7E,0x7E,0x81,0x81,0x99,0x99,0xA5,0xA5,0xA5,0xA5,0x99,0x99,0x81,0x81,0x7E,0x7E
  // 0x00, 0x00,
  // 0xff, 0x00,
  // 0x00, 0xff,
  // 0xff, 0xff,
  // 0x00, 0x00,
  // 0x00, 0x00,
  // 0x00, 0x00,
  // 0x00, 0x00,
};

const UBYTE black[] = {
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
  0xff, 0xff,
};

const UBYTE white_tile[] = {
  0x00
};

const UBYTE black_tile[] = {
  0x01
};

void init();
void set_background();

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
  set_background();

  HIDE_WIN;
  HIDE_SPRITES;
  SHOW_BKG;
}

void set_background()
{
  //int x, y;

  //set_bkg_data(0, 1, white);
  //set_bkg_data(1, 1, black);
  set_bkg_data(0, 405, title_tiles);

  set_bkg_tiles(0, 0, 20, 18, title_map);

  // for (x = 0; x < 32; x++) {
  //   for (y = 0; y < 32; y++) {
  //     if ((x + y) % 2 == 1) {
  //       set_bkg_tiles(x, y, 1, 1, black_tile);
  //     }
  //   }
  // }
}

// vim: ts=2 sw=2
