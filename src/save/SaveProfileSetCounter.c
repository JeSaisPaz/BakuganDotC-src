// bdc 0x0880d58c SaveProfileSetCounter
#include "bdc.h"

/* Stores `value`, clamped by `SaveProfileClampCounter`, into counter `idx` (0..2) of the profile
   block; does nothing if the block is missing or `idx` is out of range. */

void SaveProfileSetCounter(SaveProfile *self, s32 idx, s32 value)

{
  s32 clamped;
  
  if (((self->data != (SaveProfileData *)0x0) && (-1 < idx)) && (idx < 3)) {
    clamped = value;
    SaveProfileClampCounter(self,idx,&clamped);
    self->data->counters[idx] = clamped;
  }
  return;
}

