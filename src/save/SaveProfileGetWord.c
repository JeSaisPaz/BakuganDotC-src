// bdc 0x0880d06c SaveProfileGetWord
#include "bdc.h"

/* Returns word `index` of the profile holder's secondary 300-byte word table (`profile->+4`), or 0
   if that table is not allocated. */

u32 SaveProfileGetWord(SaveProfile *self, s32 index)

{
  u32 word;
  
  word = 0;
  if (self->words != (u32 *)0x0) {
    word = self->words[index];
  }
  return word;
}

