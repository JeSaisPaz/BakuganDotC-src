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
    boxMtx.x.x = VfDot4(mtx->x.x, mtx->x.y, mtx->x.z, mtx->x.w, a->x.x, a->y.x, a->z.x, a->w.x);
    boxMtx.x.y = VfDot4(mtx->x.x, mtx->x.y, mtx->x.z, mtx->x.w, a->x.y, a->y.y, a->z.y, a->w.y);
    boxMtx.x.z = VfDot4(mtx->x.x, mtx->x.y, mtx->x.z, mtx->x.w, a->x.z, a->y.z, a->z.z, a->w.z);
    boxMtx.x.w = VfDot4(mtx->x.x, mtx->x.y, mtx->x.z, mtx->x.w, a->x.w, a->y.w, a->z.w, a->w.w);
    boxMtx.y.x = VfDot4(mtx->y.x, mtx->y.y, mtx->y.z, mtx->y.w, a->x.x, a->y.x, a->z.x, a->w.x);
    boxMtx.y.y = VfDot4(mtx->y.x, mtx->y.y, mtx->y.z, mtx->y.w, a->x.y, a->y.y, a->z.y, a->w.y);
    boxMtx.y.z = VfDot4(mtx->y.x, mtx->y.y, mtx->y.z, mtx->y.w, a->x.z, a->y.z, a->z.z, a->w.z);
    boxMtx.y.w = VfDot4(mtx->y.x, mtx->y.y, mtx->y.z, mtx->y.w, a->x.w, a->y.w, a->z.w, a->w.w);
    boxMtx.z.x = VfDot4(mtx->z.x, mtx->z.y, mtx->z.z, mtx->z.w, a->x.x, a->y.x, a->z.x, a->w.x);
    boxMtx.z.y = VfDot4(mtx->z.x, mtx->z.y, mtx->z.z, mtx->z.w, a->x.y, a->y.y, a->z.y, a->w.y);
    boxMtx.z.z = VfDot4(mtx->z.x, mtx->z.y, mtx->z.z, mtx->z.w, a->x.z, a->y.z, a->z.z, a->w.z);
    boxMtx.z.w = VfDot4(mtx->z.x, mtx->z.y, mtx->z.z, mtx->z.w, a->x.w, a->y.w, a->z.w, a->w.w);
    boxMtx.w.x = VfDot4(mtx->w.x, mtx->w.y, mtx->w.z, mtx->w.w, a->x.x, a->y.x, a->z.x, a->w.x);
    boxMtx.w.y = VfDot4(mtx->w.x, mtx->w.y, mtx->w.z, mtx->w.w, a->x.y, a->y.y, a->z.y, a->w.y);
    boxMtx.w.z = VfDot4(mtx->w.x, mtx->w.y, mtx->w.z, mtx->w.w, a->x.z, a->y.z, a->z.z, a->w.z);
    boxMtx.w.w = VfDot4(mtx->w.x, mtx->w.y, mtx->w.z, mtx->w.w, a->x.w, a->y.w, a->z.w, a->w.w);
  }
  CollisionDebugAddBox(&(self->aabbMin).x, &(self->aabbMax).x, colour, &boxMtx);
}
