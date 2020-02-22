#include <gb/font.h>
#include <gb/gb.h>
#include <rand.h>
#include <stdlib.h>
#include "../tile-data/tileset.h"
#include "types.h"

#define MIN(x,y) ((x) > (y) ? (y) : (x))
#define MAX(x,y) ((x) < (y) ? (y) : (x))

state_t state;

struct _cookie {
  struct _tile {
    UBYTE x;
    UBYTE y;
  } tile;
  pos_t x;
  pos_t y;
} cookie;

const UBYTE refresh = 5;
const WORD acceleration = 0x0002;
const UBYTE scroll_boundary = 40;

#define max_speed ((WORD) 0x00a0)
#define min_speed ((UWORD) 0xff60U)

void init();
void init_interface();
void init_sprites();
void init_cookie();

void read_input();
void move_gumdrop();
void scroll_background();

void animate_sprites();

void show_font();
void printc_win(UBYTE x, UBYTE y, char c);
void prints_win(UBYTE x, UBYTE y, char *c);
void printi_win(UBYTE x, UBYTE y, UWORD i);

int intersects_with_gumball(pos_t *x, pos_t *y);

int intersects_with_gumball(pos_t *x, pos_t *y) {
  return
    abs(state.background.x.b.h + state.gumdrop.x.b.h - x->b.h) < 16
    && abs(state.background.y.b.h + state.gumdrop.y.b.h - y->b.h) < 16;
}

void main()
{
  init();

  while(TRUE) {
    delay(refresh);
    state.frame_counter++;

    read_input();
    move_gumdrop();
    animate_sprites();

    if ((state.frame_counter % 8) == 0) {
      printi_win(-2, 0, state.background.x.b.h);
      printi_win(-2, 1, state.background.y.b.h);

      printi_win(4, 0, state.gumdrop.x.b.h);
      printi_win(4, 1, state.gumdrop.y.b.h);

      printi_win(10, 0, state.background.x.b.h + state.gumdrop.x.b.h);
      printi_win(10, 1, state.background.y.b.h + state.gumdrop.y.b.h);

      if (intersects_with_gumball(&cookie.x, &cookie.y)) {
        prints_win(16, 0, "x");
      } else {
        prints_win(16, 0, "o");
      }
    }

    // background scrolls during VBL
  }
}

void init() {
  disable_interrupts();
  DISPLAY_OFF;

  initarand(0);

  font_init();
  font_load(font_min);

  set_win_data(0x26, 26, tileset);
  set_sprite_data(0, 8, robot_sprite);

  state.gumdrop.x.b.h = state.gumdrop.y.b.h = 75;

  init_interface();
  init_sprites();

  add_VBL(scroll_background);

  init_cookie();

  //show_font();

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

void show_font() {
  prints_win(0, 0, "AZ az 0 9");

  printi_win(0, 1, 65535);

  printi_win(6, 1, 0);

  printi_win(12, 1, -1);
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
    state.gumdrop.speed.x.w = MAX(state.gumdrop.speed.x.w - acceleration, min_speed);
  } else if (j & J_RIGHT) {
    state.gumdrop.speed.x.w = MIN(state.gumdrop.speed.x.w + acceleration, max_speed);
  } else if (state.gumdrop.speed.x.w > 0) {
    state.gumdrop.speed.x.w -= MIN(acceleration, state.gumdrop.speed.x.w);
  } else if (state.gumdrop.speed.x.w < 0) {
    state.gumdrop.speed.x.w -= MAX(-acceleration, state.gumdrop.speed.x.w);
  }

  if (j & J_UP) {
    state.gumdrop.speed.y.w = MAX(state.gumdrop.speed.y.w - acceleration, min_speed);
  } else if (j & J_DOWN) {
    state.gumdrop.speed.y.w = MIN(state.gumdrop.speed.y.w + acceleration, max_speed);
  } else if (state.gumdrop.speed.y.w > 0) {
    state.gumdrop.speed.y.w -= MIN(acceleration, state.gumdrop.speed.y.w);
  } else if (state.gumdrop.speed.y.w < 0) {
    state.gumdrop.speed.y.w -= MAX(-acceleration, state.gumdrop.speed.y.w);
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
