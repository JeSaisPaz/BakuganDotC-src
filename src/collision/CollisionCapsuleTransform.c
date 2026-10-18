// bdc 0x089e97a0 CollisionCapsuleTransform
#include "bdc.h"

/* Transform method of the capsule shape (vtable `0x08af5624` entry 6): writes into `out` the axis
   start `+0x20` transformed by `mtx` as a point (w = 1) and the axis vector `+0x30` as a vector,
   then copies the radius `+0x40`, radius² `+0x2c`, axis length `+0x3c` and the type word `+0x0`.
   The listing's `sv.q` also writes a fourth lane over `+0x2c` and `+0x3c`; both are overwritten by
   the copies that follow, so the C leaves those lanes out. */

void CollisionCapsuleTransform(void *capsule, const ScePspFMatrix4 *mtx, void *out)

{
  const CollisionCapsule *src = capsule;
  CollisionCapsule *dst = out;
  float px = src->start[0];
  float py = src->start[1];
  float pz = src->start[2];
  float ax;
  float ay;
  float az;

  /* vtfm4.q C000, E100, C200 with S203 = 1.0 (vfim 0x3c00) */
  dst->start[0] = VfDot4(mtx->x.x, mtx->y.x, mtx->z.x, mtx->w.x, px, py, pz, 1.0f);
  dst->start[1] = VfDot4(mtx->x.y, mtx->y.y, mtx->z.y, mtx->w.y, px, py, pz, 1.0f);
  dst->start[2] = VfDot4(mtx->x.z, mtx->y.z, mtx->z.z, mtx->w.z, px, py, pz, 1.0f);

  /* vtfm3.t C000, E100, C200 */
  ax = src->axis[0];
  ay = src->axis[1];
  az = src->axis[2];
  dst->axis[0] = VfDot3(mtx->x.x, mtx->y.x, mtx->z.x, ax, ay, az);
  dst->axis[1] = VfDot3(mtx->x.y, mtx->y.y, mtx->z.y, ax, ay, az);
  dst->axis[2] = VfDot3(mtx->x.z, mtx->y.z, mtx->z.z, ax, ay, az);

  dst->radius = src->radius;
  dst->radiusSq = src->radiusSq;
  dst->axisLen = src->axisLen;
  dst->type = src->type;
}
