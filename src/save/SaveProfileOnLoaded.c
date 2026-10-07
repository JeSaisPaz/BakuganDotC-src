// bdc 0x0880d418 SaveProfileOnLoaded
#include "bdc.h"

/* Post-load fixup used by `SaveLoadTaskUpdate`: applies the loaded volume options
   (`SaveProfileApplyVolumes`) and clears the session word table (`SaveProfileResetWordTable`
   with `keepFlag = 1`). */

void SaveProfileOnLoaded(SaveProfile *self)

{
  SaveProfileApplyVolumes(self);
  SaveProfileResetWordTable(self,true);
  return;
}

