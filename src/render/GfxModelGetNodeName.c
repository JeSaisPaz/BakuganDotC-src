// bdc 0x089decac GfxModelGetNodeName
#include "bdc.h"

/* Returns the name of node `index` (`model+0xfc[index]`) or NULL when out of range (`model+0xe8`).
    */

const char *GfxModelGetNodeName(GfxModel *self, s32 index)

{
  if ((-1 < index) && (index < self->nodeCount)) {
    return self->nodeChunks[index];
  }
  return (char *)0x0;
}

