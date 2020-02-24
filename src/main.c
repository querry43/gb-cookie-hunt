#include <gb/gb.h>
#include "game.h"
#include "menu.h"
#include "title.h"
#include "score.h"

void main()
{
  show_title();

  while (TRUE) {
    show_menu();
    start_game();
    show_score();
  }
}

// vim: ts=2 sw=2
