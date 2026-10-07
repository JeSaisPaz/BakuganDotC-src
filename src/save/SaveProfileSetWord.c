// bdc 0x0880cd5c SaveProfileSetWord
#include "bdc.h"

/* Stores `value` into word `index` of the profile holder's word table (`profile->+4`, 300 bytes)
   when that table exists, then clamps it with `SaveProfileClampWord`. Counterpart of
   `SaveProfileGetWord`; 61 callers. Word 0 is a bit set of profile flags (`SaveProfileSetFlags`
   / `SaveProfileHasFlags`). */

void SaveProfileSetWord(SaveProfile *self, s32 index, u32 value)

{
  if (self->words != (u32 *)0x0) {
    self->words[index] = value;
    SaveProfileClampWord(self,index);
  }
  return;
}

