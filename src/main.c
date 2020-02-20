#include <gb/gb.h>
#include <stdio.h>
#include "../tile-data/tileset.h"

#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define MAX(x,y) ((x) < (y) ? (y) : (x))

typedef union _pos {
  struct {
    UBYTE l;
    UBYTE h;
  } b;
  WORD w;
} pos;

pos x_speed, y_speed;
pos gumdrop_x_pos, gumdrop_y_pos;
pos background_x_pos, background_y_pos;

const UBYTE refresh = 5;
const WORD acceleration = 0x0002;
const UBYTE scroll_boundary = 40;

#define max_speed ((WORD) 0x00a0)
#define min_speed ((UWORD) 0xff60U)

void init();
void init_interface();
void init_sprites();

void read_input();
void move_gumdrop();
void scroll_background();

void main()
{
  init();

  while(TRUE) {
    delay(refresh);

    read_input();
    move_gumdrop();

    // background scrolls during VBL
  }
}

void init() {
  disable_interrupts();
  DISPLAY_OFF;

  set_win_data(0, 26, tileset);
  set_sprite_data(0, 4, robot_sprite);

  x_speed.w = x_speed.w = 0x0000;

  gumdrop_x_pos.w = gumdrop_y_pos.w = 0x4b00;
  background_x_pos.w = background_x_pos.w = 0;

  init_interface();
  init_sprites();

  add_VBL(scroll_background);

  SHOW_BKG;
  SHOW_WIN;
  SHOW_SPRITES;
  DISPLAY_ON;
  enable_interrupts();
}

void init_sprites() {
  SPRITES_8x16;

  set_sprite_tile(0, 0x00);
  set_sprite_tile(1, 0x02);
  move_sprite(0, gumdrop_x_pos.b.h, gumdrop_y_pos.b.h);
  move_sprite(1, gumdrop_x_pos.b.h + 8, gumdrop_y_pos.b.h);
}

void init_interface() {
  int x, y;

  for (x = 0; x < 32; x += 4)
    for (y = 0; y < 32; y += 4)
      set_bkg_tiles(x, y, 4, 4, background_map);

  // just some context, remove me
  set_bkg_tiles(2, 0, 2, 2, cookie_map);
  set_bkg_tiles(16, 6, 2, 2, cookie_map);
  set_bkg_tiles(13, 11, 2, 2, cookie_map);

  set_win_tiles(0, 0, 20, 2, hud_map);

  move_win(7, 128);
}

void read_input() {
  char j = joypad();

  // set gumdrop's speed, limiting it to the min and max speeds
  if (j & J_LEFT) {
    x_speed.w = MAX(x_speed.w - acceleration, min_speed);
  } else if (j & J_RIGHT) {
    x_speed.w = MIN(x_speed.w + acceleration, max_speed);
  }

  if (j & J_UP) {
    y_speed.w = MAX(y_speed.w - acceleration, min_speed);
  } else if (j & J_DOWN) {
    y_speed.w = MIN(y_speed.w + acceleration, max_speed);
  }
}

// scroll gumdrop or the background if gumdrop is close to the edge
void move_gumdrop() {
  if (gumdrop_x_pos.b.h > 160 - scroll_boundary && x_speed.w > 0) {
    background_x_pos.w += x_speed.w;
  } else if (gumdrop_x_pos.b.h < scroll_boundary && x_speed.w < 0) {
    background_x_pos.w += x_speed.w;
  } else {
    gumdrop_x_pos.w += x_speed.w;
  }

  if (gumdrop_y_pos.b.h > 160 - 16 - scroll_boundary && y_speed.w > 0) {
    background_y_pos.w += y_speed.w;
  } else if (gumdrop_y_pos.b.h < scroll_boundary && y_speed.w < 0) {
    background_y_pos.w += y_speed.w;
  } else {
    gumdrop_y_pos.w += y_speed.w;
  }

  move_sprite(0, gumdrop_x_pos.b.h, gumdrop_y_pos.b.h);
  move_sprite(1, gumdrop_x_pos.b.h + 8, gumdrop_y_pos.b.h);
}

void scroll_background() NONBANKED {
  move_bkg(background_x_pos.b.h, background_y_pos.b.h);
}

// vim: ts=2 sw=2
