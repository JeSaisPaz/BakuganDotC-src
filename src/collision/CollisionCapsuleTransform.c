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
  dst->start[0] = mtx->x.x * px + mtx->y.x * py + mtx->z.x * pz + mtx->w.x * 1.0f;
  dst->start[1] = mtx->x.y * px + mtx->y.y * py + mtx->z.y * pz + mtx->w.y * 1.0f;
  dst->start[2] = mtx->x.z * px + mtx->y.z * py + mtx->z.z * pz + mtx->w.z * 1.0f;

  /* vtfm3.t C000, E100, C200 */
  ax = src->axis[0];
  ay = src->axis[1];
  az = src->axis[2];
  dst->axis[0] = mtx->x.x * ax + mtx->y.x * ay + mtx->z.x * az;
  dst->axis[1] = mtx->x.y * ax + mtx->y.y * ay + mtx->z.y * az;
  dst->axis[2] = mtx->x.z * ax + mtx->y.z * ay + mtx->z.z * az;

  dst->radius = src->radius;
  dst->radiusSq = src->radiusSq;
  dst->axisLen = src->axisLen;
  dst->type = src->type;
}
