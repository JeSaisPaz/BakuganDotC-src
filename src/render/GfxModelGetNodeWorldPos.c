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
    out->x = VfDot4(b[12], b[13], b[14], b[15], a[0], a[4], a[8], a[12]);
    out->y = VfDot4(b[12], b[13], b[14], b[15], a[1], a[5], a[9], a[13]);
    out->z = VfDot4(b[12], b[13], b[14], b[15], a[2], a[6], a[10], a[14]);
    out->w = VfDot4(b[12], b[13], b[14], b[15], a[3], a[7], a[11], a[15]);
    return;
  }
  *out = g_gfxVecZero;
}
