// bdc 0x089df0bc GfxModelFindMaterialIndexBySubstr
#include "bdc.h"

/* Returns the index of the first material whose name contains `substr` (`strstr` over
   `model+0x104`, count `+0xf0`), or -1. */

s32 GfxModelFindMaterialIndexBySubstr(GfxModel *self, const char *substr)
{
  s32 i;

  for (i = 0; i < self->materialCount; i++) {
    if (strstr(((char **)self->materialChunks)[i], substr) != NULL) {
      return i;
    }
  }
  return -1;
}
