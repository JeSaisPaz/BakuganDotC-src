// bdc 0x089ea814 CollisionBoxDebugDraw
#include "bdc.h"

/* Debug-draw method of the box shape (type-6 slot 6): multiplies the box transform by `mtx`
   (column-major `transform · mtx`, VFPU `vmmul`; a plain copy when `mtx` is null) and adds a box
   wireframe (`CollisionDebugAddBox`) of the extents with that matrix. */

void CollisionBoxDebugDraw(CollisionBox *self, const ScePspFVector4 *colour, const ScePspFMatrix4 *mtx)

{
  const ScePspFMatrix4 *a = &self->transform;
  ScePspFMatrix4 boxMtx __attribute__((aligned(16)));

  if (mtx == (ScePspFMatrix4 *)0x0) {
    boxMtx = self->transform;
  } else {
    boxMtx.x.x = mtx->x.x * a->x.x + mtx->x.y * a->y.x + mtx->x.z * a->z.x + mtx->x.w * a->w.x;
    boxMtx.x.y = mtx->x.x * a->x.y + mtx->x.y * a->y.y + mtx->x.z * a->z.y + mtx->x.w * a->w.y;
    boxMtx.x.z = mtx->x.x * a->x.z + mtx->x.y * a->y.z + mtx->x.z * a->z.z + mtx->x.w * a->w.z;
    boxMtx.x.w = mtx->x.x * a->x.w + mtx->x.y * a->y.w + mtx->x.z * a->z.w + mtx->x.w * a->w.w;
    boxMtx.y.x = mtx->y.x * a->x.x + mtx->y.y * a->y.x + mtx->y.z * a->z.x + mtx->y.w * a->w.x;
    boxMtx.y.y = mtx->y.x * a->x.y + mtx->y.y * a->y.y + mtx->y.z * a->z.y + mtx->y.w * a->w.y;
    boxMtx.y.z = mtx->y.x * a->x.z + mtx->y.y * a->y.z + mtx->y.z * a->z.z + mtx->y.w * a->w.z;
    boxMtx.y.w = mtx->y.x * a->x.w + mtx->y.y * a->y.w + mtx->y.z * a->z.w + mtx->y.w * a->w.w;
    boxMtx.z.x = mtx->z.x * a->x.x + mtx->z.y * a->y.x + mtx->z.z * a->z.x + mtx->z.w * a->w.x;
    boxMtx.z.y = mtx->z.x * a->x.y + mtx->z.y * a->y.y + mtx->z.z * a->z.y + mtx->z.w * a->w.y;
    boxMtx.z.z = mtx->z.x * a->x.z + mtx->z.y * a->y.z + mtx->z.z * a->z.z + mtx->z.w * a->w.z;
    boxMtx.z.w = mtx->z.x * a->x.w + mtx->z.y * a->y.w + mtx->z.z * a->z.w + mtx->z.w * a->w.w;
    boxMtx.w.x = mtx->w.x * a->x.x + mtx->w.y * a->y.x + mtx->w.z * a->z.x + mtx->w.w * a->w.x;
    boxMtx.w.y = mtx->w.x * a->x.y + mtx->w.y * a->y.y + mtx->w.z * a->z.y + mtx->w.w * a->w.y;
    boxMtx.w.z = mtx->w.x * a->x.z + mtx->w.y * a->y.z + mtx->w.z * a->z.z + mtx->w.w * a->w.z;
    boxMtx.w.w = mtx->w.x * a->x.w + mtx->w.y * a->y.w + mtx->w.z * a->z.w + mtx->w.w * a->w.w;
  }
  CollisionDebugAddBox(&(self->aabbMin).x, &(self->aabbMax).x, colour, &boxMtx);
}
