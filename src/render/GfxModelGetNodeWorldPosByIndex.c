// bdc 0x089dee80 GfxModelGetNodeWorldPosByIndex
#include "bdc.h"

/* Index variant of `GfxModelGetNodeWorldPos`: writes the world translation of node `index`
   (`GfxModelGetNode`), the translation column of `data->rootMatrix * node->localMatrix` (VFPU
   vmmul), to `out`, or the default vector `g_gfxVecZero` when there is no such node. */

void GfxModelGetNodeWorldPosByIndex(GfxModel *self, ScePspFVector4 *out, s32 index)
{
  GmoNode *node;
  const float *a;
  const float *b;

  node = GfxModelGetNode(self, index);

  if (node != NULL) {
    a = self->data->rootMatrix;
    b = node->localMatrix;
    out->x = VfDot4(b[12], b[13], b[14], b[15], a[0], a[4], a[8], a[12]);
    out->y = VfDot4(b[12], b[13], b[14], b[15], a[1], a[5], a[9], a[13]);
    out->z = VfDot4(b[12], b[13], b[14], b[15], a[2], a[6], a[10], a[14]);
    out->w = VfDot4(b[12], b[13], b[14], b[15], a[3], a[7], a[11], a[15]);
    return;
  }
  *out = g_gfxVecZero;
}
