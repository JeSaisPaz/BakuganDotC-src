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
  m.x.x = mtx->x.x * a->x.x + mtx->x.y * a->y.x + mtx->x.z * a->z.x + mtx->x.w * a->w.x;
  m.x.y = mtx->x.x * a->x.y + mtx->x.y * a->y.y + mtx->x.z * a->z.y + mtx->x.w * a->w.y;
  m.x.z = mtx->x.x * a->x.z + mtx->x.y * a->y.z + mtx->x.z * a->z.z + mtx->x.w * a->w.z;
  m.x.w = mtx->x.x * a->x.w + mtx->x.y * a->y.w + mtx->x.z * a->z.w + mtx->x.w * a->w.w;
  m.y.x = mtx->y.x * a->x.x + mtx->y.y * a->y.x + mtx->y.z * a->z.x + mtx->y.w * a->w.x;
  m.y.y = mtx->y.x * a->x.y + mtx->y.y * a->y.y + mtx->y.z * a->z.y + mtx->y.w * a->w.y;
  m.y.z = mtx->y.x * a->x.z + mtx->y.y * a->y.z + mtx->y.z * a->z.z + mtx->y.w * a->w.z;
  m.y.w = mtx->y.x * a->x.w + mtx->y.y * a->y.w + mtx->y.z * a->z.w + mtx->y.w * a->w.w;
  m.z.x = mtx->z.x * a->x.x + mtx->z.y * a->y.x + mtx->z.z * a->z.x + mtx->z.w * a->w.x;
  m.z.y = mtx->z.x * a->x.y + mtx->z.y * a->y.y + mtx->z.z * a->z.y + mtx->z.w * a->w.y;
  m.z.z = mtx->z.x * a->x.z + mtx->z.y * a->y.z + mtx->z.z * a->z.z + mtx->z.w * a->w.z;
  m.z.w = mtx->z.x * a->x.w + mtx->z.y * a->y.w + mtx->z.z * a->z.w + mtx->z.w * a->w.w;
  m.w.x = mtx->w.x * a->x.x + mtx->w.y * a->y.x + mtx->w.z * a->z.x + mtx->w.w * a->w.x;
  m.w.y = mtx->w.x * a->x.y + mtx->w.y * a->y.y + mtx->w.z * a->z.y + mtx->w.w * a->w.y;
  m.w.z = mtx->w.x * a->x.z + mtx->w.y * a->y.z + mtx->w.z * a->z.z + mtx->w.w * a->w.z;
  m.w.w = mtx->w.x * a->x.w + mtx->w.y * a->y.w + mtx->w.z * a->z.w + mtx->w.w * a->w.w;
  out->transform = m;
  self->invValid = 0;
}
