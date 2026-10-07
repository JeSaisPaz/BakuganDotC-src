// bdc 0x0880d7c8 SaveProfileClearFlags
#include "bdc.h"

/* Clears the bits of `mask` in profile word 0 (the profile flag set): `SaveProfileSetWord(profile,
   0, SaveProfileGetWord(profile, 0) & ~mask)`. Counterpart of `SaveProfileSetFlags` and
   `SaveProfileHasFlags`. */

void SaveProfileClearFlags(SaveProfile *self, u32 mask)

{
  u32 flags;
  
  flags = SaveProfileGetWord(self,0);
  SaveProfileSetWord(self,0,flags & ~mask);
  return;
}

