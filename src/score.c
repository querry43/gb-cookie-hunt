#include <gb/gb.h>
#include "../tile-data/tileset.menu.h"
#include "config.h"
#include "score.h"

void show_score() {
  disable_interrupts();
  DISPLAY_OFF;

  set_bkg_data(menu_tileset_offset, menu_tileset_size, menu_tileset);
  set_bkg_tiles(0, 0, 20, 18, score_menu_map);

  move_bkg(0, 0);

  HIDE_SPRITES;
  HIDE_WIN;
  SHOW_BKG;

  DISPLAY_ON;
}

// vim: ts=2 sw=2
