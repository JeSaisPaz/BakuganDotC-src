// bdc 0x089e8f94 CollisionSphereOverlapSphere
#include "bdc.h"

/* Returns whether two spheres (centre, radius) overlap: the squared centre distance is at most the
   squared sum of the radii. */

bool CollisionSphereOverlapSphere(const CollisionSphere *a, const CollisionSphere *b)
{
  float dx = VfSub(a->center[0], b->center[0]);
  float dy = VfSub(a->center[1], b->center[1]);
  float dz = VfSub(a->center[2], b->center[2]);
  float distSq = VfDot3(dx, dy, dz, dx, dy, dz);
  float r = a->radius + b->radius;

  return distSq <= r * r;
}
