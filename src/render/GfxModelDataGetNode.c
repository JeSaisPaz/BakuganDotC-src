// bdc 0x08a15918 GfxModelDataGetNode
#include "bdc.h"

/* Returns the node record `index` of a GMO model data block: `*(modelData+4) + index * 0xc0` if
   `modelData` is non-NULL and `index < *(u16 *)(modelData+0x18)`, NULL otherwise; if `index + 1`
   has a non-zero upper half-word (`index` >= 0xffff and not -1, e.g. a value that already looks
   like a pointer) `index` is returned unchanged; -1 (`index + 1 == 0`) fails the bounds check and
   returns NULL. */

GmoNode *GfxModelDataGetNode(GmoModel *data, u32 index)

{
  if (data != (GmoModel *)0x0) {
    if ((index + 1 & 0xffff0000) != 0) {
      return (GmoNode *)PspPtr(index);
    }
    if ((index & 0xffff) < (uint)data->nodeCount) {
      return data->nodes + index;
    }
  }
  return (GmoNode *)0x0;
}

