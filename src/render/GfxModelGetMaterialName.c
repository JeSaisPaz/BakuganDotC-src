// bdc 0x089df204 GfxModelGetMaterialName
#include "bdc.h"

/* Returns the name of material `index` (`model+0x104[index]`), or NULL when out of range
   (`model+0xf0`). */

const char *GfxModelGetMaterialName(GfxModel *self, s32 index)

{
  if ((-1 < index) && (index < self->materialCount)) {
    return self->materialChunks[index];
  }
  return (char *)0x0;
}

