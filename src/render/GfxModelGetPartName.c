// bdc 0x089defe4 GfxModelGetPartName
#include "bdc.h"

/* Returns entry `index` of the model's type-5 chunk name table (`+0x100`, count `+0xec`, see
   `GfxModelIndexChunks`) or NULL when out of range. */

const char *GfxModelGetPartName(GfxModel *self, s32 index)

{
  if ((-1 < index) && (index < self->partCount)) {
    return self->partChunks[index];
  }
  return (char *)0x0;
}

