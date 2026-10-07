// bdc 0x0880d718 SaveProfileSubWord
#include "bdc.h"

/* Subtracts `amount` from profile word `index` (stored back through `SaveProfileSetWord`, so it
   is clamped) and returns the old value; returns 0 without the word table. Counterpart of
   `SaveProfileAddWord`; used by `BtlMainPhaseBattle`. */

s32 SaveProfileSubWord(SaveProfile *self, s32 index, s32 amount)
{
  u32 old;

  old = 0;
  if (self->words != (u32 *)0x0) {
    old = self->words[index];
    SaveProfileSetWord(self, index, old - amount);
  }
  return old;
}
