// bdc 0x089ded30 GfxModelFindNode
#include "bdc.h"

/* Looks up a node (bone/object) of a GMO model instance by name: `GfxModelFindNodeIndex` finds
   the index in the model's name table and `GfxModelGetNode`-style access returns the node record
   (`0xc0` bytes each) from the model data at `model+0x130`; returns NULL when the name does not
   exist. Used with names like `"Bip01_Spine"`, `"spell"`, `"context00"`, `"context01"`. */

void *GfxModelFindNode(GfxModel *self, const char *name)

{
  s32 index;

  index = GfxModelFindNodeIndex(self,name);
  if (index >= 0) {
    return GfxModelDataGetNode(self->data, index);
  }
  return NULL;
}

