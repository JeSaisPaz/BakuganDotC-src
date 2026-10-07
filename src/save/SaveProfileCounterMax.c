// bdc 0x0880d4d0 SaveProfileCounterMax
#include "bdc.h"

/* Upper bound of profile counter `idx`: idx 0 -> 3599999 (999:59:59 expressed in seconds, the
   play-time cap), idx 1 -> 1, any other idx (including negative) -> 0. `profile` is ignored. */

s32 SaveProfileCounterMax(SaveProfile *self, s32 idx)

{
  s32 max;
  
  max = 0;
  if (idx < 1) {
    if (-1 < idx) {
      max = 3599999;
    }
  }
  else if (idx < 2) {
    return 1;
  }
  return max;
}

