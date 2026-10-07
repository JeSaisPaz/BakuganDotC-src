// bdc 0x088f53ec GameEventFlagClear
#include "bdc.h"

/* Clears story event flag `id` (16-bit, valid below 2000): counter-class ids (see
   `GameEventFlagTest`) decrement the shared counter byte at `0x08b00bd9`, flooring at 0; other
   ids clear bit `id & 31` of word `id >> 5` of the bitset at `0x08b00bdc`. */

void GameEventFlagClear(u32 id)
{
  u32 *bits;
  s32 n;

  id &= 0xffff;
  if (id < 2000) {
    if (GameEventFlagInRange(2, (u16)id) != 0) {
      n = g_gameEventFlags[5] - 1;
      g_gameEventFlags[5] = (n >= 0) ? n : 0;
      return;
    }
    bits = (u32 *)&g_gameEventFlags[8];
    bits[(s32)id >> 5] &= ~(1 << (id & 0x1f));
  }
}
