// bdc 0x089ea2f0 CollisionBoxTransform
#include "bdc.h"

/* Transform method of the box shape (vtable `0x08af5684` entry 6): copies the extents `+0x10` and
   `+0x20` into `out`, stores the box transform `+0x30` multiplied by `mtx` (column-major
   `transform · mtx`, VFPU `vmmul`) as `out+0x30`, and clears the cached-inverse flag `+0xb0` of
   the source box. The product is formed in full before it is stored, so `out` may be `self`. */

void CollisionBoxTransform(CollisionBox *self, const ScePspFMatrix4 *mtx, CollisionBox *out)

{
  const ScePspFMatrix4 *a = &self->transform;
  ScePspFMatrix4 m;

  out->aabbMin = self->aabbMin;
  out->aabbMax = self->aabbMax;
  m.x.x = VfDot4(mtx->x.x, mtx->x.y, mtx->x.z, mtx->x.w, a->x.x, a->y.x, a->z.x, a->w.x);
  m.x.y = VfDot4(mtx->x.x, mtx->x.y, mtx->x.z, mtx->x.w, a->x.y, a->y.y, a->z.y, a->w.y);
  m.x.z = VfDot4(mtx->x.x, mtx->x.y, mtx->x.z, mtx->x.w, a->x.z, a->y.z, a->z.z, a->w.z);
  m.x.w = VfDot4(mtx->x.x, mtx->x.y, mtx->x.z, mtx->x.w, a->x.w, a->y.w, a->z.w, a->w.w);
  m.y.x = VfDot4(mtx->y.x, mtx->y.y, mtx->y.z, mtx->y.w, a->x.x, a->y.x, a->z.x, a->w.x);
  m.y.y = VfDot4(mtx->y.x, mtx->y.y, mtx->y.z, mtx->y.w, a->x.y, a->y.y, a->z.y, a->w.y);
  m.y.z = VfDot4(mtx->y.x, mtx->y.y, mtx->y.z, mtx->y.w, a->x.z, a->y.z, a->z.z, a->w.z);
  m.y.w = VfDot4(mtx->y.x, mtx->y.y, mtx->y.z, mtx->y.w, a->x.w, a->y.w, a->z.w, a->w.w);
  m.z.x = VfDot4(mtx->z.x, mtx->z.y, mtx->z.z, mtx->z.w, a->x.x, a->y.x, a->z.x, a->w.x);
  m.z.y = VfDot4(mtx->z.x, mtx->z.y, mtx->z.z, mtx->z.w, a->x.y, a->y.y, a->z.y, a->w.y);
  m.z.z = VfDot4(mtx->z.x, mtx->z.y, mtx->z.z, mtx->z.w, a->x.z, a->y.z, a->z.z, a->w.z);
  m.z.w = VfDot4(mtx->z.x, mtx->z.y, mtx->z.z, mtx->z.w, a->x.w, a->y.w, a->z.w, a->w.w);
  m.w.x = VfDot4(mtx->w.x, mtx->w.y, mtx->w.z, mtx->w.w, a->x.x, a->y.x, a->z.x, a->w.x);
  m.w.y = VfDot4(mtx->w.x, mtx->w.y, mtx->w.z, mtx->w.w, a->x.y, a->y.y, a->z.y, a->w.y);
  m.w.z = VfDot4(mtx->w.x, mtx->w.y, mtx->w.z, mtx->w.w, a->x.z, a->y.z, a->z.z, a->w.z);
  m.w.w = VfDot4(mtx->w.x, mtx->w.y, mtx->w.z, mtx->w.w, a->x.w, a->y.w, a->z.w, a->w.w);
  out->transform = m;
  self->invValid = 0;
}
