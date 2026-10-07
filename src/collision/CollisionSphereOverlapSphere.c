// bdc 0x089e8f94 CollisionSphereOverlapSphere
#include "bdc.h"

/* Returns whether two spheres (centre, radius) overlap: the squared centre distance is at most the
   squared sum of the radii. */

bool CollisionSphereOverlapSphere(const CollisionSphere *a, const CollisionSphere *b)
{
  float dx = a->center[0] - b->center[0];
  float dy = a->center[1] - b->center[1];
  float dz = a->center[2] - b->center[2];
  float distSq = dx * dx + dy * dy + dz * dz;
  float r = a->radius + b->radius;

  return distSq <= r * r;
}
