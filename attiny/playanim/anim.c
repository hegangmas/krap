/*** This is the file where all the animation content is stored. If you
     just want to change what pictures show up, in what order and/or for
     how long, this is the place to look.
***/

#include <avr/pgmspace.h>
#include "anim.h"

/* _step[] is an array of bytes that acts as a control script for the
   animation.

   Most byte values are just an instruction to display the corresponding
   image from _frame[] (see below) for the current delay time.

   Byte value 255 is a special marker that means the following byte is
   a new standard delay (in milliseconds). 

   This means that if we want to repeat an image in an animation cycle,
   we only need to store the bitmap once; we can then reference it as many
   times as we want. It also means we only need to provide the delay time
   when it changes rather than between every frame.

   You can have at most 255 bitmaps and the longest possible delay is
   255ms. You can get longer delays by repeating the same image several
   times in a row. To remove these restrictions entirely, change this to
   an array of 16-bit words (and make corresponding changes in the calling
   code). */
const uint8_t PROGMEM _step[] = {
  255, 250, // delay 250 ms between frames
  0, 1, 0, 1, // alien 0
  2, 3, 2, 3, // alien 1
  4, 5, 4, 5, // alien 2
  255, 125, // change delay to 125ms per frame
  6, 7, 8, 9, 6, 7, 8, 9 // alien 3
};

/* _frame[] is an array of bytes, each of which represents a row of pixels
   on the 8x8 LED matrix. '1' bits represent a lit LED; '0' means dark.

   The rows are arranged into groups of 8, each of which represents a single
   8x8 bitmap image. The first 8 bytes (i.e., array elements with indices 0-7)
   correspond to a _step[] value of 0. The next 8, a _step[] value of 1 and
   so on.

   The values in this example are characters from the classic 1978 Midway
   coin-op video game "Space Invaders" by Tomohiro Nishikado.

   The bitmap data was copied from the "Trinket / Gemma Space Invader
   Pendant" project by Phillip Burgess. See the original here:

 http://learn.adafruit.com/trinket-slash-gemma-space-invader-pendant/animation
*/
const uint8_t PROGMEM _frame[] = {

  0b00011000, // index 0: alien 0 frame 0
  0b00111100,
  0b01111110,
  0b11011011,
  0b11111111,
  0b00100100,
  0b01011010,
  0b10100101,

  0b00011000, // index 1: alien 0 frame 1
  0b00111100,
  0b01111110,
  0b11011011,
  0b11111111,
  0b00100100,
  0b01011010,
  0b01000010,

  0b00000000, // index 2: alien 1 frame 0
  0b00111100,
  0b01111110,
  0b11011011,
  0b11011011,
  0b01111110,
  0b00100100,
  0b11000011,

  0b00111100, // index 3: alien 1 frame 1
  0b01111110,
  0b11011011,
  0b11011011,
  0b01111110,
  0b00100100,
  0b00100100,
  0b00100100,

  0b00100100, // index 4: alien 2 frame 0
  0b00100100,
  0b01111110,
  0b11011011,
  0b11111111,
  0b11111111,
  0b10100101,
  0b00100100,

  0b00100100, // index 5: alien 2 frame 1
  0b10100101,
  0b11111111,
  0b11011011,
  0b11111111,
  0b01111110,
  0b00100100,
  0b01000010,

  0b00111100, // index 6: alien 3 frame 0
  0b01111110,
  0b00110011,
  0b01111110,
  0b00111100,
  0b00000000,
  0b00001000,
  0b00000000,

  0b00111100, // index 7: alien 3 frame 1
  0b01111110,
  0b10011001,
  0b01111110,
  0b00111100,
  0b00000000,
  0b00001000,
  0b00001000,

  0b00111100, // index 8: alien 3 frame 2
  0b01111110,
  0b11001100,
  0b01111110,
  0b00111100,
  0b00000000,
  0b00000000,
  0b00001000,

  0b00111100, // index 9: alien 3 frame 3
  0b01111110,
  0b01100110,
  0b01111110,
  0b00111100,
  0b00000000,
  0b00000000,
  0b00000000
};

/* Function so external stuff can see the size of the step array. The
   sizeof() value is a compile-time constant, so this should end up
   being an immediate constant value when compiled... */
int anim_steps(void) { return sizeof(_step); }
