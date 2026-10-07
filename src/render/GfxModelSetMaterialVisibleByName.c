// bdc 0x089e0780 GfxModelSetMaterialVisibleByName
#include "bdc.h"

/* Sets or clears the visible bit (0x20 of state byte `+4`) of the first material whose name
   contains `name` (`GfxModelSetMaterialVisible`); returns 1 when found. */

bool GfxModelSetMaterialVisibleByName(GfxModel *self, const char *name, bool visible)

{
  s32 index;
  
  index = GfxModelFindMaterialIndexBySubstr(self,name);
  if (index != -1) {
    GfxModelSetMaterialVisible(self,index,visible);
    return true;
  }
  return false;
}

