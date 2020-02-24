#include <gb/gb.h>
#include "config.h"
#include "score.h"
#include "text.h"
#include "tile_maps.h"
#include "game.h"

const unsigned char score_line1[] = " Final Score";

void show_score() {
  disable_interrupts();
  DISPLAY_OFF;

  init_text();

  move_win(7, 0);

  set_win_tiles(0, 0, 20, 18, white_map);

  HIDE_SPRITES;
  SHOW_BKG;
  SHOW_WIN;

  prints_win(3, 4, score_line1);
  printi_win(7, 7, game_state.score);

  DISPLAY_ON;

  waitpadup();
  waitpad(J_START | J_SELECT | J_A | J_B);
}

// vim: ts=2 sw=2
