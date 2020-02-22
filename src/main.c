#include <gb/gb.h>
#include <stdio.h>
#include "../tile-data/tileset.h"
#include "types.h"

#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define MAX(x,y) ((x) < (y) ? (y) : (x))

state_t state;

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

void animate_sprites();

void main()
{

  init();

  while(TRUE) {
    delay(refresh);
    state.frame_counter++;

    read_input();
    move_gumdrop();
    animate_sprites();

    // background scrolls during VBL
  }
}

void init() {
  disable_interrupts();
  DISPLAY_OFF;

  set_win_data(0, 26, tileset);
  set_sprite_data(0, 8, robot_sprite);

  state.gumdrop.x.b.h = state.gumdrop.y.b.h = 75;

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
  set_sprite_tile(1, 0x04);
  move_sprite(0, state.gumdrop.x.b.h, state.gumdrop.y.b.h);
  move_sprite(1, state.gumdrop.x.b.h + 8, state.gumdrop.y.b.h);
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
    state.gumdrop.speed.x.w = MAX(state.gumdrop.speed.x.w - acceleration, min_speed);
  } else if (j & J_RIGHT) {
    state.gumdrop.speed.x.w = MIN(state.gumdrop.speed.x.w + acceleration, max_speed);
  }

  if (j & J_UP) {
    state.gumdrop.speed.y.w = MAX(state.gumdrop.speed.y.w - acceleration, min_speed);
  } else if (j & J_DOWN) {
    state.gumdrop.speed.y.w = MIN(state.gumdrop.speed.y.w + acceleration, max_speed);
  }
}

// scroll gumdrop or the background if gumdrop is close to the edge
void move_gumdrop() {
  if (state.gumdrop.x.b.h > 160 - scroll_boundary && state.gumdrop.speed.x.w > 0) {
    state.background.x.w += state.gumdrop.speed.x.w;
  } else if (state.gumdrop.x.b.h < scroll_boundary && state.gumdrop.speed.x.w < 0) {
    state.background.x.w += state.gumdrop.speed.x.w;
  } else {
    state.gumdrop.x.w += state.gumdrop.speed.x.w;
  }

  if (state.gumdrop.y.b.h > 160 - 16 - scroll_boundary && state.gumdrop.speed.y.w > 0) {
    state.background.y.w += state.gumdrop.speed.y.w;
  } else if (state.gumdrop.y.b.h < scroll_boundary && state.gumdrop.speed.y.w < 0) {
    state.background.y.w += state.gumdrop.speed.y.w;
  } else {
    state.gumdrop.y.w += state.gumdrop.speed.y.w;
  }

  move_sprite(0, state.gumdrop.x.b.h, state.gumdrop.y.b.h);
  move_sprite(1, state.gumdrop.x.b.h + 8, state.gumdrop.y.b.h);
}

void scroll_background() {
  move_bkg(state.background.x.b.h, state.background.y.b.h);
}

void animate_sprites() {
  if ((state.frame_counter % 20) == 0) {
    set_sprite_tile(0, 0x00 + (state.gumdrop.sprite_offset * 2));
    set_sprite_tile(1, 0x04 + (state.gumdrop.sprite_offset * 2));

    state.gumdrop.sprite_offset = (state.gumdrop.sprite_offset + 1) % 2;
  }
}

// vim: ts=2 sw=2
