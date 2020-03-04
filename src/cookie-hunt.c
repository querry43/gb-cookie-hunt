#include <gb/gb.h>
#include "game.h"
#include "menu.h"
#include "title.h"
#include "score.h"
#include "sound.h"

void main()
{
  init_sound();

  while (TRUE) {
    show_title();
    show_menu();
    start_game();
    show_score();
  }
}

// vim: ts=2 sw=2
