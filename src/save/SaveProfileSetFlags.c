// bdc 0x0880d0a8 SaveProfileSetFlags
#include "bdc.h"

/* Sets the bits of `mask` in profile word 0 (`SaveProfileSetWord(profile, 0,
   SaveProfileGetWord(profile, 0) | mask)`). 23 callers; `NetPlayLateUpdate` and the NetPlay state
   handlers use it to set flag `0x80` when a session dies (e.g. UMD removed). */

void SaveProfileSetFlags(SaveProfile *self, u32 mask)

{
  u32 flags;
  
  flags = SaveProfileGetWord(self,0);
  SaveProfileSetWord(self,0,mask | flags);
  return;
}

