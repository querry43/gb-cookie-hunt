#ifndef __config_h__
#define __config_h__

#define DEBUG FALSE

#define acceleration ((WORD) 0x0008)
#define scroll_boundary ((UBYTE) 40)

#define num_cookies 4

#define num_barriers 8
#define barrier_length 10
#define max_num_barrier_segments 96

#define cookie_intersect_distance ((UBYTE) 10)
#define barrier_intersect_distance ((UBYTE) 10)

#define max_speed ((WORD) 0x0190)
#define min_speed ((UWORD) 0xfe70U)

#define game_tileset_offset 38
#define game_tileset_size 32

#define menu_tileset_offset 0
#define menu_tileset_size 86

#define title_tileset_offset 0
#define title_tileset_size 200

#define slow_text_delay 50

#define cookie_energy_capacity 10

#define song_delay 200

#endif
