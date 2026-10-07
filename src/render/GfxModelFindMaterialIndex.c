// bdc 0x089df018 GfxModelFindMaterialIndex
#include "bdc.h"

/* Returns the index of the material named exactly `name` (`strcmp` over `model+0x104`, count
   `+0xf0`), or -1. */

s32 GfxModelFindMaterialIndex(GfxModel *self, const char *name)
{
  s32 i;

  for (i = 0; i < self->materialCount; i++) {
    if (strcmp(((char **)self->materialChunks)[i], name) == 0) {
      return i;
    }
  }
  return -1;
}
