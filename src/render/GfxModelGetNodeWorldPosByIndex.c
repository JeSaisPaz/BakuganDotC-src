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
    out->x = b[12] * a[0] + b[13] * a[4] + b[14] * a[8] + b[15] * a[12];
    out->y = b[12] * a[1] + b[13] * a[5] + b[14] * a[9] + b[15] * a[13];
    out->z = b[12] * a[2] + b[13] * a[6] + b[14] * a[10] + b[15] * a[14];
    out->w = b[12] * a[3] + b[13] * a[7] + b[14] * a[11] + b[15] * a[15];
    return;
  }
  *out = g_gfxVecZero;
}
