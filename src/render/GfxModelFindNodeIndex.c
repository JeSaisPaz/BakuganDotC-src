// bdc 0x089dec08 GfxModelFindNodeIndex
#include "bdc.h"

/* Linear search of the GMO model instance's node-name table (`model+0xfc`, array of `char *`, count
   at `model+0xe8`) for `name` with `strcmp`; returns the index of the first match or -1. */

s32 GfxModelFindNodeIndex(GfxModel *self, const char *name)
{
  s32 i;

  for (i = 0; i < self->nodeCount; i++) {
    if (strcmp(((char **)self->nodeChunks)[i], name) == 0) {
      return i;
    }
  }
  return -1;
}
