// bdc 0x089ded7c GfxModelFindNodeRecord
#include "bdc.h"

/* Finds a node by name (`GfxModelFindNodeIndex`) and returns its record from the model data
   (`GfxModelDataGetNode`), or NULL. Same result as `GfxModelFindNode`. */

void *GfxModelFindNodeRecord(GfxModel *self, const char *name)

{
  s32 index;

  index = GfxModelFindNodeIndex(self,name);
  if (index >= 0) {
    return GfxModelDataGetNode(self->data, index);
  }
  return NULL;
}

