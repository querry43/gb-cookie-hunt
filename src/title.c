#include <gb/gb.h>
#include "../tile-data/tileset.title.h"
#include "config.h"
#include "sound.h"
#include "title.h"

void show_title() {
  disable_interrupts();
  DISPLAY_OFF;

  set_bkg_data(title_tileset_offset, title_tileset_size, title_tileset);
  set_bkg_tiles(0, 0, 20, 18, title_title_map);

  move_bkg(0, 0);

  HIDE_SPRITES;
  HIDE_WIN;
  SHOW_BKG;

  DISPLAY_ON;

  play_title_song();

  waitpad(J_START | J_SELECT | J_A | J_B);
}

// vim: ts=2 sw=2
