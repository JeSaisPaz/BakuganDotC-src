// bdc 0x0880d6cc SaveProfileAddWord
#include "bdc.h"

/* Adds `delta` to word `index` of the profile's 300-byte word table through `SaveProfileSetWord`
   (which clamps the result) and returns the value the word had *before* the addition; returns 0 and
   does nothing when the word table (`profile + 4`) is not allocated. */

s32 SaveProfileAddWord(SaveProfile *self, s32 index, s32 delta)
{
  u32 old;

  old = 0;
  if (self->words != (u32 *)0x0) {
    old = self->words[index];
    SaveProfileSetWord(self, index, delta + old);
  }
  return old;
}
