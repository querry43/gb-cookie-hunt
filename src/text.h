#ifndef __text_h__
#define __text_h__

// loads text tiles from 0x00 to 0x25
// call before loading other bkg tiles
void init_text();

void printc_win(UBYTE x, UBYTE y, char c);
void prints_win(UBYTE x, UBYTE y, char *c);
void printi_win(UBYTE x, UBYTE y, UWORD i);

#endif
