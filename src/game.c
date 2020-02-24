#include <gb/gb.h>
#include <rand.h>
#include <stdlib.h>

#include "../tile-data/tileset.game.h"
#include "config.h"
#include "game.h"
#include "text.h"

#define min(x,y) ((x) > (y) ? (y) : (x))
#define max(x,y) ((x) < (y) ? (y) : (x))

#define gumdrop_x() (game_state.background.x.b.h + game_state.gumdrop.x.b.h)
#define gumdrop_y() (game_state.background.y.b.h + game_state.gumdrop.y.b.h)

void init();
void init_interface();
void init_sprites();
void init_cookies();
void place_cookie(UBYTE);
void remove_cookie(UBYTE);
void place_background(UBYTE, UBYTE);

void read_input();
void move_gumdrop();
void scroll_background();
void test_intersection();

void animate_sprites();

#define intersects_with_gumball(obj) \
  ((abs(gumdrop_x() - obj.x.b.h) < intersect_distance) \
  && (abs(gumdrop_y() - obj.y.b.h) < intersect_distance))

state_t game_state;

void start_game()
{
  init();

  while(TRUE) {
    // debugging
    // if (game_state.cookie_count >= 5) {
    //   return;
    // }

    wait_vbl_done();
    game_state.frame_counter++;

    read_input();
    move_gumdrop();
    animate_sprites();

    // background scrolls during VBL

    if ((game_state.frame_counter % 32) == 0) {
      game_state.score += 1U;
    }

    if ((game_state.frame_counter % 32) == 0) {
      game_state.energy_level--;
      if (game_state.energy_level == 0) {
        return;
      }
    }

    if ((game_state.frame_counter % 8) == 0) {
      if (DEBUG) {
        printi_win(-2, 0, gumdrop_x());
        printi_win(-2, 1, gumdrop_y());
      }

      //printi_win(6, 1, game_state.cookie_count);
      if (game_state.energy_level > 75) {
        prints_win(4, 1, "IooooI");
      } else if (game_state.energy_level > 50) {
        prints_win(4, 1, "Iooo I");
      } else if (game_state.energy_level > 25) {
        prints_win(4, 1, "Ioo  I");
      } else {
        prints_win(4, 1, "Io   I");
      }

      printi_win(14, 1, game_state.score);

      test_intersection();
    }
  }
}

void test_intersection() {
  int i;

  for (i = 0; i < num_cookies; i++) {
    if (intersects_with_gumball(game_state.cookies[i])) {
      remove_cookie(i);
      place_cookie(i);

      game_state.cookie_count++;

      game_state.energy_level = min(
        game_state.energy_level + cookie_energy_capacity,
        100
      );

      game_state.score += 10;
    }
  }
}

void init() {
  disable_interrupts();
  DISPLAY_OFF;
  HIDE_BKG;
  HIDE_SPRITES;
  HIDE_WIN;

  initarand(0);

  init_text();

  set_bkg_data(game_tileset_offset, game_tileset_size, game_tileset);
  set_sprite_data(0, 8, robot_game_sprite);

  game_state.background.x.w = game_state.background.y.w = 0;

  game_state.gumdrop.x.b.h = game_state.gumdrop.y.b.h = 75;
  game_state.gumdrop.speed.x.w = game_state.gumdrop.speed.y.w = 0;

  game_state.cookie_count = 0;
  game_state.energy_level = 75;
  game_state.score = 0;

  init_interface();
  init_sprites();

  add_VBL(scroll_background);

  init_cookies();

  SHOW_BKG;
  SHOW_WIN;
  SHOW_SPRITES;
  DISPLAY_ON;
  enable_interrupts();
}

void init_cookies() {
  int i;

  for (i = 0; i < num_cookies; i++)
    place_cookie(i);
}

void place_cookie(UBYTE i) {
  game_state.cookies[i].tile.x = arand() & 0x1f;
  game_state.cookies[i].tile.y = arand() & 0x1f;
  game_state.cookies[i].x.b.h = (game_state.cookies[i].tile.x * 8) + 8;
  game_state.cookies[i].y.b.h = (game_state.cookies[i].tile.y * 8) + 16;
  set_bkg_tiles(game_state.cookies[i].tile.x, game_state.cookies[i].tile.y, 2, 2, cookie_game_map);
}

void remove_cookie(UBYTE i) {
  int x, y;

  for (y = 0; y < 2; y++)
    for (x = 0; x < 2; x++)
      place_background(game_state.cookies[i].tile.x + x, game_state.cookies[i].tile.y + y);
}

void place_background(UBYTE x, UBYTE y) {
  set_bkg_tiles(x, y, 1, 1, background_game_map + (x % 4) + ((y % 4) * 4));
}

void init_sprites() {
  SPRITES_8x16;

  set_sprite_tile(0, 0x00);
  set_sprite_tile(1, 0x04);
  move_sprite(0, game_state.gumdrop.x.b.h, game_state.gumdrop.y.b.h);
  move_sprite(1, game_state.gumdrop.x.b.h + 8, game_state.gumdrop.y.b.h);
}

void init_interface() {
  int x, y;

  for (x = 0; x < 32; x += 4)
    for (y = 0; y < 32; y += 4)
      set_bkg_tiles(x, y, 4, 4, background_game_map);

  set_win_tiles(0, 0, 20, 2, hud_game_map);

  prints_win(4, 0, "Energy");
  prints_win(14, 0, "Score");

  move_win(7, 128);
}

void read_input() {
  char j = joypad();

  // set gumdrop's speed, limiting it to the min and max speeds
  if (j & J_LEFT) {
    game_state.gumdrop.speed.x.w = max(game_state.gumdrop.speed.x.w - acceleration, min_speed);
  } else if (j & J_RIGHT) {
    game_state.gumdrop.speed.x.w = min(game_state.gumdrop.speed.x.w + acceleration, max_speed);
  } else if (game_state.gumdrop.speed.x.w > 0) {
    game_state.gumdrop.speed.x.w -= min(acceleration, game_state.gumdrop.speed.x.w);
  } else if (game_state.gumdrop.speed.x.w < 0) {
    game_state.gumdrop.speed.x.w -= max(-acceleration, game_state.gumdrop.speed.x.w);
  }

  if (j & J_UP) {
    game_state.gumdrop.speed.y.w = max(game_state.gumdrop.speed.y.w - acceleration, min_speed);
  } else if (j & J_DOWN) {
    game_state.gumdrop.speed.y.w = min(game_state.gumdrop.speed.y.w + acceleration, max_speed);
  } else if (game_state.gumdrop.speed.y.w > 0) {
    game_state.gumdrop.speed.y.w -= min(acceleration, game_state.gumdrop.speed.y.w);
  } else if (game_state.gumdrop.speed.y.w < 0) {
    game_state.gumdrop.speed.y.w -= max(-acceleration, game_state.gumdrop.speed.y.w);
  }
}

// scroll gumdrop or the background if gumdrop is close to the edge
void move_gumdrop() {
  if (game_state.gumdrop.x.b.h > 160 - scroll_boundary && game_state.gumdrop.speed.x.w > 0) {
    game_state.background.x.w += game_state.gumdrop.speed.x.w;
  } else if (game_state.gumdrop.x.b.h < scroll_boundary && game_state.gumdrop.speed.x.w < 0) {
    game_state.background.x.w += game_state.gumdrop.speed.x.w;
  } else {
    game_state.gumdrop.x.w += game_state.gumdrop.speed.x.w;
  }

  if (game_state.gumdrop.y.b.h > 160 - 16 - scroll_boundary && game_state.gumdrop.speed.y.w > 0) {
    game_state.background.y.w += game_state.gumdrop.speed.y.w;
  } else if (game_state.gumdrop.y.b.h < scroll_boundary && game_state.gumdrop.speed.y.w < 0) {
    game_state.background.y.w += game_state.gumdrop.speed.y.w;
  } else {
    game_state.gumdrop.y.w += game_state.gumdrop.speed.y.w;
  }

  move_sprite(0, game_state.gumdrop.x.b.h, game_state.gumdrop.y.b.h);
  move_sprite(1, game_state.gumdrop.x.b.h + 8, game_state.gumdrop.y.b.h);
}

void scroll_background() {
  move_bkg(game_state.background.x.b.h, game_state.background.y.b.h);
}

void animate_sprites() {
  if ((game_state.frame_counter % 64) == 0) {
    set_sprite_tile(0, 0x00 + (game_state.gumdrop.sprite_offset * 2));
    set_sprite_tile(1, 0x04 + (game_state.gumdrop.sprite_offset * 2));

    game_state.gumdrop.sprite_offset = (game_state.gumdrop.sprite_offset + 1) % 2;
  }
}

// vim: ts=2 sw=2
