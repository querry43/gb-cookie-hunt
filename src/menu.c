#include <gb/gb.h>
#include "config.h"
#include "game.h"
#include "menu.h"
#include "text.h"
#include "tile_maps.h"
#include "types.h"

const unsigned char menu_line1[] = " Help Gumdrop find  ";
const unsigned char menu_line2[] = "  web cookies to    ";
const unsigned char menu_line3[] = "  replenish energy  ";

const unsigned char menu_line4[] = "Keep Gumdrop moving ";
const unsigned char menu_line5[] = " as long as you can ";

const unsigned char menu_line6[] = "Level 1";
const unsigned char menu_line7[] = "Level 2";
const unsigned char menu_line8[] = "Level 3";

const unsigned char select[] = "o";
const unsigned char unselect[] = " ";

game_mode_t selected_mode;


void show_menu() {
  UBYTE x, y;
  UBYTE j;

  disable_interrupts();
  DISPLAY_OFF;

  init_text();

  move_win(7, 0);

  set_win_tiles(0, 0, 20, 18, white_map);

  HIDE_SPRITES;
  SHOW_BKG;
  SHOW_WIN;

  DISPLAY_ON;

  for (x = 1; x < 18; x++) {
    printc_win(x, 0, *(menu_line1 + x));
    delay(slow_text_delay);
  }

  for (x = 2; x < 16; x++) {
    printc_win(x, 1, *(menu_line2 + x));
    delay(slow_text_delay);
  }

  for (x = 2; x < 18; x++) {
    printc_win(x, 2, *(menu_line3 + x));
    delay(slow_text_delay);
  }

  for (x = 0; x < 8; x++)
    delay(slow_text_delay);

  for (x = 0; x < 19; x++) {
    printc_win(x, 4, *(menu_line4 + x));
    delay(slow_text_delay);
  }

  for (x = 1; x < 19; x++) {
    printc_win(x, 5, *(menu_line5 + x));
    delay(slow_text_delay);
  }

  for (x = 0; x < 8; x++)
    delay(slow_text_delay);

  prints_win(7, 9, menu_line6);
  prints_win(7, 10, menu_line7);
  prints_win(7, 11, menu_line8);

  selected_mode = LEVEL1;

  enable_interrupts();

  while(TRUE) {
    for (y = 0; y < 3; y++) {
      if (y == selected_mode)
        prints_win(5, y + 9, select);
      else
        prints_win(5, y + 9, unselect);
    }

    waitpadup();
    j = waitpad(J_UP | J_DOWN | J_START | J_SELECT | J_A | J_B);

    if (j & J_UP) {
      if (selected_mode == 0)
        selected_mode = 2;
      else
        selected_mode--;
    } else if (j & J_DOWN) {
      if (selected_mode == 2)
        selected_mode = 0;
      else
        selected_mode++;
    } else if (j & (J_START | J_SELECT | J_A | J_B)) {
      break;
    }
  }

  game_state.game_mode = selected_mode;
}

// vim: ts=2 sw=2
