// bdc 0x089e9180 CollisionSphereVsSphere
#include "bdc.h"

/* Sphere shape method for sphere queries (vtable `0x08af55c4` entry 3): overlaps when the squared
   centre distance is at most the squared sum of the radii; on a hit writes to `out` the point on
   the centre line midway between the two surfaces (the centre of `sphere` plus the unit direction
   to `other`, scaled by `sphere` radius + half the gap), with `out->w` = 0 (bank S713). When the
   centres coincide the direction is zero and `out` is the centre of `sphere`.
   Returns the overlap flag; on a miss `out` is untouched. */

s32 CollisionSphereVsSphere(const CollisionSphere *sphere, const CollisionSphere *other,
                             ScePspFVector4 *out)
{
  float dx = VfSub(other->center[0], sphere->center[0]);
  float dy = VfSub(other->center[1], sphere->center[1]);
  float dz = VfSub(other->center[2], sphere->center[2]);
  float distSq = VfDot3(dx, dy, dz, dx, dy, dz);
  float sum = sphere->radius + other->radius;
  bool hit = distSq <= sum * sum;

  if (hit) {
    float mid = sphere->radius + (__builtin_sqrtf(distSq) - sum) * 0.5f;
    float lenSq;
    float inv;
    float nx;
    float ny;
    float nz;

    dx = VfSub(other->center[0], sphere->center[0]);
    dy = VfSub(other->center[1], sphere->center[1]);
    dz = VfSub(other->center[2], sphere->center[2]);
    lenSq = VfDot3(dx, dy, dz, dx, dy, dz);
    inv = VfRsq(lenSq);
    if (VfIsZero(lenSq))
      inv = 0.0f; /* vcmovt from bank S713 */
    nx = VfSat1(VfMul(dx, inv));
    ny = VfSat1(VfMul(dy, inv));
    nz = VfSat1(VfMul(dz, inv));
    out->x = VfMul(nx, mid);
    out->y = VfMul(ny, mid);
    out->z = VfMul(nz, mid);
    out->w = 0.0f; /* bank S713, the w lane of C710 */
    out->x = VfAdd(out->x, sphere->center[0]);
    out->y = VfAdd(out->y, sphere->center[1]);
    out->z = VfAdd(out->z, sphere->center[2]);
  }
  return hit;
}
