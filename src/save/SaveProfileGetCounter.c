// bdc 0x0880d4a4 SaveProfileGetCounter
#include "bdc.h"

/* Returns profile counter `idx` (0..2) from the 3-entry counter table of the profile block
   (`*profile + 8 + idx*4`; idx 0 = play time in seconds, idx 1 = a 0/1 counter, idx 2 = unused), or
   0 if the profile block is missing or `idx` is out of range. */

s32 SaveProfileGetCounter(SaveProfile *self, s32 idx)

{
  s32 count;
  
  count = 0;
  if (((self->data != (SaveProfileData *)0x0) && (-1 < idx)) && (idx < 3)) {
    count = self->data->counters[idx];
  }
  return count;
}

