// bdc 0x089df160 GfxModelFindMaterialIndexBySubstr2
#include "bdc.h"

/* Compiled copy of `GfxModelFindMaterialIndexBySubstr` (first material whose name contains
   `substr`, or -1), used by `GfxModelSetMaterialAnimCallback`. */

s32 GfxModelFindMaterialIndexBySubstr2(GfxModel *self, const char *substr)
{
  s32 i;

  for (i = 0; i < self->materialCount; i++) {
    if (strstr(((char **)self->materialChunks)[i], substr) != NULL) {
      return i;
    }
  }
  return -1;
}
