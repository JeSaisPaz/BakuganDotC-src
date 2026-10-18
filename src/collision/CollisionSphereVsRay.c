// bdc 0x089e9040 CollisionSphereVsRay
#include "bdc.h"

/* Sphere shape method for ray queries (type-3 vtable slot 0): ray/sphere intersection (quadratic on
   `b = d·m`, `c = m·m - r²` with `m` = origin - centre), writing `dir * t` (w = 0, bank S713) to `out`
   and returning 1 on a hit; 0 when the origin is outside and pointing away, or the discriminant is
   negative. */

bool CollisionSphereVsRay(const void *sphere, const void *ray, ScePspFVector4 *out)
{
  const CollisionSphere *sph = sphere;
  const CollisionRayShape *r = ray;
  float mx = VfSub(r->origin.x, sph->center[0]);
  float my = VfSub(r->origin.y, sph->center[1]);
  float mz = VfSub(r->origin.z, sph->center[2]);
  float b = VfDot3(mx, my, mz, r->dir.x, r->dir.y, r->dir.z);
  float c = VfDot3(mx, my, mz, mx, my, mz);
  float disc;
  float t;

  c = c - sph->radius * sph->radius;
  if (!(c <= 0.0f) && !(b <= 0.0f)) {
    return false;
  }
  disc = b * b - c;
  if (disc < 0.0f) {
    return false;
  }
  t = -b - __builtin_sqrtf(disc);
  out->x = VfMul(r->dir.x, t);
  out->y = VfMul(r->dir.y, t);
  out->z = VfMul(r->dir.z, t);
  out->w = 0.0f; /* bank S713: vmul.t writes only xyz of C710 */
  return true;
}
