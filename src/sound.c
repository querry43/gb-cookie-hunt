#include <gb/gb.h>
#include "config.h"
#include "sound.h"

// https://gist.github.com/gingemonster/600c33f7dd97ecbf785eca8c84772c9a
// http://www.devrs.com/gb/files/sndtab.html
//
// channel 1: quadrangular wave pattern with sweep and envelope functions
// NR10_REG - register 0, Frequency sweep settings
// NR11_REG - register 1: Wave pattern duty and sound length
// NR12_REG - register 2: Volume Envelope (Makes the volume get louder or quieter each "tick")
// NR13_REG - register 3: Frequency LSbs (Least Significant bits) and noise options
// NR14_REG - register 4: Playback and frequency MSbs


// song
// C 1546
// F 1673
// A 1750
// G 1714
// F 1673
// E 1650
// D 1602
// E 1650
// F 1673

#define TONE_C5 1546
#define TONE_D5 1602
#define TONE_E5 1650
#define TONE_F5 1673
#define TONE_G5 1714
#define TONE_A5 1750

void play_tone(UWORD tone) {
  NR10_REG = 0x00;

          // duty | length
  NR11_REG = 0xC0 | 0x00;

          // vol  | dir  | step
  NR12_REG = 0x80 | 0x00 | 0x03;

  // tone lsb
  NR13_REG = (tone & 0xff);

          // enbl | tone msb
  NR14_REG = 0x80 | ((tone & 0x0700) >> 8);
}

void play_tone_sweep(UWORD tone, UBYTE down) {
  // sweep
          // time | dir  | shift
  NR10_REG = 0x70 | (down << 3) | 0x07;

          // duty | length
  NR11_REG = 0xC0 | 0x00;

          // vol  | dir  | step
  NR12_REG = 0x80 | 0x00 | 0x03;

  // tone lsb
  NR13_REG = (tone & 0xff);

          // enbl | tone msb
  NR14_REG = 0x80 | ((tone & 0x0700) >> 8);
}

void play_eat_cookie() {
  play_tone_sweep(TONE_C5, FALSE);
}

void play_game_over() {
  play_tone_sweep(TONE_E5, TRUE);
  delay(song_delay*2);
  play_tone_sweep(TONE_D5, TRUE);
  delay(song_delay*2);
  play_tone_sweep(TONE_C5, TRUE);
}

void play_title_song() {
  play_tone(TONE_C5);
  delay(song_delay * 2);

  play_tone(TONE_F5);
  delay(song_delay * 2);

  play_tone(TONE_A5);
  delay(song_delay * 1);

  play_tone(TONE_G5);
  delay(song_delay * 1);

  play_tone(TONE_F5);
  delay(song_delay * 2);

  play_tone(TONE_E5);
  delay(song_delay * 2);

  play_tone(TONE_D5);
  delay(song_delay * 2);

  play_tone(TONE_E5);
  delay(song_delay * 2);

  play_tone(TONE_F5);
}

void init_sound() {
  NR52_REG = 0x80; // enable sound
  NR51_REG = 0xff; // enable all channels
  NR50_REG = 0x77; // max volume
}

// vim: ts=2 sw=2
