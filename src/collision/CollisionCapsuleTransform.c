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
  ScePspFVector4 t;
  const ScePspFVector4 zero = {0.0f, 0.0f, 0.0f, 0.0f};

  /* vtfm4.q C000, E100, C200 with S203 = 1.0 (vfim 0x3c00) */
  VfDot4x4(&t.x, &mtx->x.x, &mtx->y.x, &mtx->z.x, &mtx->w.x, px, py, pz, 1.0f); /* lane w unused */
  dst->start[0] = t.x;
  dst->start[1] = t.y;
  dst->start[2] = t.z;

  /* vtfm3.t C000, E100, C200 */
  ax = src->axis[0];
  ay = src->axis[1];
  az = src->axis[2];
  VfDot4x4(&t.x, &mtx->x.x, &mtx->y.x, &mtx->z.x, &zero.x, ax, ay, az, 0.0f); /* VfDot3: 4th term 0 x 0 */
  dst->axis[0] = t.x;
  dst->axis[1] = t.y;
  dst->axis[2] = t.z;

  dst->radius = src->radius;
  dst->radiusSq = src->radiusSq;
  dst->axisLen = src->axisLen;
  dst->type = src->type;
}
