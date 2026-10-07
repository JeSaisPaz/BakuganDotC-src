// bdc 0x08a15918 GfxModelDataGetNode
#include "bdc.h"

/* Returns the node record `index` of a GMO model data block: `*(modelData+4) + index * 0xc0` if
   `modelData` is non-NULL and `index < *(u16 *)(modelData+0x18)`, NULL otherwise; any value whose
   upper half-word is not zero (i.e. one that already looks like a pointer, or -1) is returned
   unchanged. */

GmoNode *GfxModelDataGetNode(GmoModel *data, u32 index)

{
  if (data != (GmoModel *)0x0) {
    if ((index + 1 & 0xffff0000) != 0) {
      return (GmoNode *)(uintptr_t)index;
    }
    if ((index & 0xffff) < (uint)data->nodeCount) {
      return data->nodes + index;
    }
  }
  return (GmoNode *)0x0;
}

