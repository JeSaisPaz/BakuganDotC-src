// bdc 0x089e9388 CollisionCapsuleOverlapSphere
#include "bdc.h"

/* Returns whether a capsule (segment `+0x10`, radius `+0x40`) and a sphere (centre `+0x10`, radius
   `+0x20`) overlap. */

bool CollisionCapsuleOverlapSphere(const void *capsule, const void *sphere)

{
  const CollisionCapsule *cap = capsule;
  const CollisionSphere *sph = sphere;
  float distSq;
  float r;

  distSq = SegmentDistSqToPoint((SegmentShape *)&cap->segment, sph->center);
  r = cap->radius + sph->radius;
  return distSq <= r * r;
}
