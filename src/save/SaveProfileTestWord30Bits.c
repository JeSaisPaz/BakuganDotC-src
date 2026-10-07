// bdc 0x089b1d3c SaveProfileTestWord30Bits
#include "bdc.h"

/* Returns true when any bit of `mask` is set in the low 16 bits of word 0x30 of the player
   profile's secondary word table (`SaveProfileGetWord`). */

bool SaveProfileTestWord30Bits(u16 mask)
{
  SaveProfile *self;
  u32 word;

  self = SaveGetProfile();
  word = SaveProfileGetWord(self, 0x30);
  return (word & 0xffff & mask) != 0;
}
