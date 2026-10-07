// bdc 0x0880d7a0 SaveProfileHasFlags
#include "bdc.h"

/* Returns whether any bit of `mask` is set in profile word 0 (`(mask & SaveProfileGetWord(profile,
   0)) != 0`). 26 callers; counterpart of `SaveProfileSetFlags`. */

bool SaveProfileHasFlags(SaveProfile *self, u32 mask)

{
  u32 flags;
  
  flags = SaveProfileGetWord(self,0);
  return (mask & flags) != 0;
}

