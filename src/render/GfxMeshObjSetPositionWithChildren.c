// bdc 0x088256ac GfxMeshObjSetPositionWithChildren
#include "bdc.h"

/* Copies the position `pos` (vec4) to the translation row `+0xb0` of the mesh
   object (`GfxMeshObjCtor`)'s two child objects `+0x154` and `+0x158`, then of the object itself. Used by
   `BtlShadowUpdate`. */

void GfxMeshObjSetPositionWithChildren(GfxMeshObj *self, const float *pos)
{
  float *dst;
  int i;

  dst = self->child0->pos;
  for (i = 0; i < 4; i++) {
    dst[i] = pos[i];
  }
  dst = self->u158.child1->pos;
  for (i = 0; i < 4; i++) {
    dst[i] = pos[i];
  }
  dst = self->pos;
  for (i = 0; i < 4; i++) {
    dst[i] = pos[i];
  }
}
