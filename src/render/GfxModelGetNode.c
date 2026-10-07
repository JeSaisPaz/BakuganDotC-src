// bdc 0x089dece0 GfxModelGetNode
#include "bdc.h"

/* Returns the node record of a GMO model instance by index: NULL unless `0 <= index <
   model->nodeCount` (`model+0xe8`), otherwise `GfxModelDataGetNode(*(model+0x130), index)`
   (`modelData->+4 + index * 0xc0`). */

GmoNode *GfxModelGetNode(GfxModel *self, s32 index)
{
  if (index >= 0 && index < self->nodeCount) {
    return GfxModelDataGetNode(self->data, index);
  }
  return NULL;
}
