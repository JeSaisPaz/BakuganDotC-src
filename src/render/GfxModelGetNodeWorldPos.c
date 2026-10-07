// bdc 0x089dedc8 GfxModelGetNodeWorldPos
#include "bdc.h"

/* Writes the world position of the node `name` into `out` (vec4): the translation column of
   `data->rootMatrix * node->localMatrix` (VFPU vmmul); when the node is missing writes the default
   vector `g_gfxVecZero`. */

void GfxModelGetNodeWorldPos(GfxModel *self, ScePspFVector4 *out, const char *name)
{
  GmoNode *node;
  const float *a;
  const float *b;
  s32 index;

  index = GfxModelFindNodeIndex(self, name);

  if (index >= 0) {
    node = GfxModelDataGetNode(self->data, index);
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
