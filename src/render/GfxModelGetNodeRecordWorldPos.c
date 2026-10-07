// bdc 0x089def30 GfxModelGetNodeRecordWorldPos
#include "bdc.h"

/* Node-record variant of `GfxModelGetNodeWorldPos`: writes the world translation of `node`
   (translation column of `data->rootMatrix * node->localMatrix`, VFPU vmmul) to `out`, or
   `g_gfxVecZero` when `node` is NULL. */

void GfxModelGetNodeRecordWorldPos(GfxModel *self, ScePspFVector4 *out, void *node)

{
  const float *a;
  const float *b;

  if (node != NULL) {
    a = self->data->rootMatrix;
    b = ((GmoNode *)node)->localMatrix;
    out->x = b[12] * a[0] + b[13] * a[4] + b[14] * a[8] + b[15] * a[12];
    out->y = b[12] * a[1] + b[13] * a[5] + b[14] * a[9] + b[15] * a[13];
    out->z = b[12] * a[2] + b[13] * a[6] + b[14] * a[10] + b[15] * a[14];
    out->w = b[12] * a[3] + b[13] * a[7] + b[14] * a[11] + b[15] * a[15];
    return;
  }
  *out = g_gfxVecZero;
}
