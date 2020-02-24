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
    waitpadup();
    waitpad(J_START | J_SELECT | J_A | J_B);
  }
}

// vim: ts=2 sw=2
