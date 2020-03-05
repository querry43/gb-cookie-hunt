#include <gb/gb.h>
#include <rand.h>
#include <stdlib.h>
#include "../tile-data/tileset.game.h"
#include "config.h"
#include "game.h"
#include "sound.h"
#include "text.h"

#define min(x,y) ((x) > (y) ? (y) : (x))
#define max(x,y) ((x) < (y) ? (y) : (x))

#define gumdrop_x() (game_state.background.x.b.h + game_state.gumdrop.x.b.h)
#define gumdrop_y() (game_state.background.y.b.h + game_state.gumdrop.y.b.h)

void init();
void init_interface();
void init_sprites();
void init_cookies();
void init_barriers();
void place_cookie(UBYTE);
void remove_cookie(UBYTE);
UBYTE place_h_barrier(UBYTE);
UBYTE place_v_barrier(UBYTE);
void place_barrier_segment(UBYTE i, UBYTE x, UBYTE y, UBYTE tile);
void place_background(UBYTE, UBYTE);

void read_input();
void move_gumdrop();
void scroll_background();
void test_intersection();

void animate_sprites();

#define intersects_with_gumball(obj, distance) \
  ((abs(gumdrop_x() - obj.pos.x.b.h) < distance) \
  && (abs(gumdrop_y() - obj.pos.y.b.h) < distance))

state_t game_state;
game_mode_t game_mode;

void start_game()
{
  init();

  while(TRUE) {
    // debugging
    // if (game_state.cookie_count >= 3) {
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

    if ((game_state.frame_counter % 16) == 0) {
      game_state.energy_level--;
      if (game_state.energy_level == 0) {
        play_game_over();
        return;
      }
    }

    if ((game_state.frame_counter % 8) == 0) {
      if (DEBUG) {
        printi_win(-2, 0, gumdrop_x());
        printi_win(-2, 1, gumdrop_y());
      }

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
    if (intersects_with_gumball(game_state.cookies[i], cookie_intersect_distance)) {
      remove_cookie(i);
      place_cookie(i);

      game_state.cookie_count++;

      game_state.energy_level = min(
        game_state.energy_level + cookie_energy_capacity,
        100
      );

      game_state.score += 10;

      play_eat_cookie();
    }
  }

  for (i = 0; i < game_state.num_barrier_segments; i++) {
    if (intersects_with_gumball(game_state.barrier_segments[i], barrier_intersect_distance)) {
      game_state.score = 0;
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

  if (game_mode >= LEVEL2)
    init_barriers();

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
  // cookies must be places between 0 and 30 inclusive in y and x
  // cookies should be at least 4 tiles apart
  // it is ok if two cookies are exactly ontop of eachother
  UBYTE x = arand();
  UBYTE y = arand();

  // cookie positions are 0, 4, 8, 12, 16, 20, 24
  // oo..oo..oo..oo..oo..oo..oo
  // oo..oo..oo..oo..oo..oo..oo
  // ..........................
  // ..........................
  // oo..oo..oo..oo..oo..oo..oo
  // oo..oo..oo..oo..oo..oo..oo
  // .........................
  // .........................
  game_state.cookies[i].tile.x = (x % 7) * 4;
  game_state.cookies[i].tile.y = (y % 7) * 4;

  // this position is based on the center of the cookie
  game_state.cookies[i].pos.x.b.h = (game_state.cookies[i].tile.x * 8) + 8;
  game_state.cookies[i].pos.y.b.h = (game_state.cookies[i].tile.y * 8) + 16;

  set_bkg_tiles(game_state.cookies[i].tile.x, game_state.cookies[i].tile.y, 2, 2, cookie_game_map);
}

void remove_cookie(UBYTE i) {
  int x, y;

  for (y = 0; y < 2; y++)
    for (x = 0; x < 2; x++)
      place_background(game_state.cookies[i].tile.x + x, game_state.cookies[i].tile.y + y);
}

void init_barriers() {
  UBYTE i;
  game_state.num_barrier_segments = 0;

  for (i = 0; i < num_barriers / 2; i++)
    game_state.num_barrier_segments = place_h_barrier(game_state.num_barrier_segments);

  for (i = 0; i < num_barriers / 2; i++)
    game_state.num_barrier_segments = place_v_barrier(game_state.num_barrier_segments);
}

UBYTE place_h_barrier(UBYTE barrier) {
  // barrier positions should not intersect with cookie positions
  UBYTE x = arand();
  UBYTE y = arand();
  UBYTE len = arand();

  UBYTE i;

  UBYTE x_pos = ((x % 7) * 4) + 2;
  UBYTE y_pos = ((y % 7) * 4) + 2;
  UBYTE full_len = 2 + (len % (barrier_length - 2));

  place_barrier_segment(barrier++, x_pos, y_pos, hbarrier_game_map[0]);

  for (i = 1; i < full_len - 1; i++)
    place_barrier_segment(barrier++, x_pos + i, y_pos, hbarrier_game_map[1]);

  place_barrier_segment(barrier++, x_pos + full_len - 1, y_pos, hbarrier_game_map[2]);

  return barrier;
}

UBYTE place_v_barrier(UBYTE barrier) {
  // barrier positions should not intersect with cookie positions
  UBYTE x = arand();
  UBYTE y = arand();
  UBYTE len = arand();

  UBYTE i;

  UBYTE x_pos = ((x % 7) * 4) + 2;
  UBYTE y_pos = ((y % 7) * 4) + 2;
  UBYTE full_len = 2 + (len % (barrier_length - 2));

  place_barrier_segment(barrier++, x_pos, y_pos, vbarrier_game_map[0]);

  for (i = 1; i < full_len - 1; i++)
    place_barrier_segment(barrier++, x_pos, y_pos + i, vbarrier_game_map[1]);

  place_barrier_segment(barrier++, x_pos, y_pos + full_len - 1, vbarrier_game_map[2]);

  return barrier;
}

void place_barrier_segment(UBYTE i, UBYTE x, UBYTE y, UBYTE tile) {
  unsigned char map[] = { tile };

  game_state.barrier_segments[i].tile.x = x;
  game_state.barrier_segments[i].tile.y = y;

  game_state.barrier_segments[i].pos.x.b.h = (game_state.barrier_segments[i].tile.x * 8) + 4;
  game_state.barrier_segments[i].pos.y.b.h = (game_state.barrier_segments[i].tile.y * 8) + 12;

  set_bkg_tiles(game_state.barrier_segments[i].tile.x, game_state.barrier_segments[i].tile.y, 1, 1, map);
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
