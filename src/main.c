#include <gb/gb.h>
#include <rand.h>
#include <stdlib.h>
#include "../tile-data/tileset.h"
#include "text.h"
#include "types.h"

#define DEBUG TRUE

#define acceleration ((WORD) 0x0008)
#define scroll_boundary ((UBYTE) 40)

#define intersect_distance ((UBYTE) 10)

#define max_speed ((WORD) 0x0100)
#define min_speed ((UWORD) 0xff00U)

#define min(x,y) ((x) > (y) ? (y) : (x))
#define max(x,y) ((x) < (y) ? (y) : (x))

#define gumdrop_x() (state.background.x.b.h + state.gumdrop.x.b.h)
#define gumdrop_y() (state.background.y.b.h + state.gumdrop.y.b.h)

void init();
void init_interface();
void init_sprites();
void init_cookie();

void read_input();
void move_gumdrop();
void scroll_background();

void animate_sprites();

#define intersects_with_gumball(obj) \
  ((abs(gumdrop_x() - obj.x.b.h) < intersect_distance) \
  && (abs(gumdrop_y() - obj.y.b.h) < intersect_distance))

state_t state;

// this is temporary
struct _cookie {
  struct _tile {
    UBYTE x;
    UBYTE y;
  } tile;
  pos_t x;
  pos_t y;
} cookie;

void main()
{
  init();

  while(TRUE) {
    wait_vbl_done();
    state.frame_counter++;

    read_input();
    move_gumdrop();
    animate_sprites();

    // background scrolls during VBL

    if ((state.frame_counter % 8) == 0) {
      if (DEBUG) {
        printi_win(-2, 0, gumdrop_x());
        printi_win(-2, 1, gumdrop_y());
      }

      if (intersects_with_gumball(cookie)) {
        printc_win(16, 0, 'X');
      } else {
        printc_win(16, 0, 'O');
      }
    }
  }
}

void init() {
  disable_interrupts();
  DISPLAY_OFF;

  initarand(0);

  init_text();

  set_bkg_data(0x26, 26, tileset);
  set_sprite_data(0, 8, robot_sprite);

  state.gumdrop.x.b.h = state.gumdrop.y.b.h = 75;

  init_interface();
  init_sprites();

  add_VBL(scroll_background);

  init_cookie();

  SHOW_BKG;
  SHOW_WIN;
  SHOW_SPRITES;
  DISPLAY_ON;
  enable_interrupts();
}

void init_cookie() {
  cookie.tile.x = arand() & 0x1f;
  cookie.tile.y = arand() & 0x1f;
  cookie.x.b.h = (cookie.tile.x * 8) + 8;
  cookie.y.b.h = (cookie.tile.y * 8) + 16;
  set_bkg_tiles(cookie.tile.x, cookie.tile.y, 2, 2, cookie_map);
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

  set_win_tiles(0, 0, 20, 2, hud_map);

  move_win(7, 128);
}

void read_input() {
  char j = joypad();

  // set gumdrop's speed, limiting it to the min and max speeds
  if (j & J_LEFT) {
    state.gumdrop.speed.x.w = max(state.gumdrop.speed.x.w - acceleration, min_speed);
  } else if (j & J_RIGHT) {
    state.gumdrop.speed.x.w = min(state.gumdrop.speed.x.w + acceleration, max_speed);
  } else if (state.gumdrop.speed.x.w > 0) {
    state.gumdrop.speed.x.w -= min(acceleration, state.gumdrop.speed.x.w);
  } else if (state.gumdrop.speed.x.w < 0) {
    state.gumdrop.speed.x.w -= max(-acceleration, state.gumdrop.speed.x.w);
  }

  if (j & J_UP) {
    state.gumdrop.speed.y.w = max(state.gumdrop.speed.y.w - acceleration, min_speed);
  } else if (j & J_DOWN) {
    state.gumdrop.speed.y.w = min(state.gumdrop.speed.y.w + acceleration, max_speed);
  } else if (state.gumdrop.speed.y.w > 0) {
    state.gumdrop.speed.y.w -= min(acceleration, state.gumdrop.speed.y.w);
  } else if (state.gumdrop.speed.y.w < 0) {
    state.gumdrop.speed.y.w -= max(-acceleration, state.gumdrop.speed.y.w);
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
  if ((state.frame_counter % 64) == 0) {
    set_sprite_tile(0, 0x00 + (state.gumdrop.sprite_offset * 2));
    set_sprite_tile(1, 0x04 + (state.gumdrop.sprite_offset * 2));

    state.gumdrop.sprite_offset = (state.gumdrop.sprite_offset + 1) % 2;
  }
}

// vim: ts=2 sw=2
