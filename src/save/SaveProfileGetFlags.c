// bdc 0x0880d08c SaveProfileGetFlags
#include "bdc.h"

/* Returns word 0 of the profile word table, the profile flag bit set: `SaveProfileGetWord(profile,
   0)`. Only caller is `SaveProfileResetWordTable`, which saves the flags before clearing the
   table. */

u32 SaveProfileGetFlags(SaveProfile *self)

{
  u32 flags;
  
  flags = SaveProfileGetWord(self,0);
  return flags;
}

