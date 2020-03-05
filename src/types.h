#ifndef __types_h__
#define __types_h__

#include "config.h"

typedef union _coord_t {
  struct {
    UBYTE l;
    UBYTE h;
  } b;
  WORD w;
} coord_t;

typedef struct _pos_t {
  coord_t x;
  coord_t y;
} pos_t;

typedef struct _tile_t {
  UBYTE x;
  UBYTE y;
} tile_t;

typedef struct _cookie_t {
  // tile is used for placement placement
  tile_t tile;

  // pos is used for intersection
  pos_t pos;
} cookie_t;

typedef struct _barrier_segment_t {
  tile_t tile;

  // pos is used for intersection
  pos_t pos;
} barrier_segment_t;

typedef enum _game_mode_t {
  LEVEL1 = 0,
  LEVEL2 = 1,
  LEVEL3 = 2,
} game_mode_t;


typedef struct _state_t {
  UBYTE frame_counter;

  struct _background {
    coord_t x;
    coord_t y;
  } background;

  struct _gumdrop {
    coord_t x;
    coord_t y;

    struct _speed {
      coord_t x;
      coord_t y;
    } speed;

    UBYTE sprite_offset;
  } gumdrop;

  cookie_t cookies[num_cookies];
  barrier_segment_t barrier_segments[max_num_barrier_segments];
  UBYTE num_barrier_segments;

  UBYTE cookie_count;
  UBYTE energy_level;
  UWORD score;
} state_t;

#endif

// vim: ts=2 sw=2
