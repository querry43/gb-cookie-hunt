#ifndef __types_h__
#define __types_h__

#define num_cookies 4

typedef union _pos_t {
  struct {
    UBYTE l;
    UBYTE h;
  } b;
  WORD w;
} pos_t;

typedef struct _cookie_t {
  struct _tile {
    UBYTE x;
    UBYTE y;
  } tile;

  pos_t x;
  pos_t y;
} cookie_t;

typedef enum _game_mode_t {
  LEVEL1 = 0,
  LEVEL2 = 1,
  LEVEL3 = 2,
} game_mode_t;


typedef struct _state_t {
  UBYTE frame_counter;

  game_mode_t game_mode;

  struct _background {
    pos_t x;
    pos_t y;
  } background;

  struct _gumdrop {
    pos_t x;
    pos_t y;

    struct _speed {
      pos_t x;
      pos_t y;
    } speed;

    UBYTE sprite_offset;
  } gumdrop;

  cookie_t cookies[num_cookies];

  UBYTE cookie_count;
  UBYTE energy_level;
  UWORD score;
} state_t;

#endif

// vim: ts=2 sw=2
