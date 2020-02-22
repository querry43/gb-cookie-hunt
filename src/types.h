#ifndef __types_h__
#define __types_h__

typedef union _pos_t {
  struct {
    UBYTE l;
    UBYTE h;
  } b;
  WORD w;
} pos_t;

typedef struct _state_t {
  UBYTE frame_counter;

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
} state_t;

#endif
