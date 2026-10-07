// bdc 0x089df76c GfxModelFindMaterialState
#include "bdc.h"

/* Returns the state record of the material named exactly `name` (`GfxModelFindMaterialIndex`,
   `GfxModelGetMaterialState`), or NULL. */

void *GfxModelFindMaterialState(GfxModel *self, const char *name)

{
  s32 index;

  index = GfxModelFindMaterialIndex(self,name);
  if (index != -1) {
    return GfxModelGetMaterialState(self, index);
  }
  return NULL;
}

