// bdc 0x089e9388 CollisionCapsuleOverlapSphere
#include "bdc.h"

/* Returns whether a capsule (segment `+0x10`, radius `+0x40`) and a sphere (centre `+0x10`, radius
   `+0x20`) overlap. */

bool CollisionCapsuleOverlapSphere(const void *capsule, const void *sphere)

{
  const CollisionCapsule *cap = capsule;
  const float *sph = sphere;
  float distSq;
  float r;

  distSq = SegmentDistSqToPoint((SegmentShape *)&cap->segmentHead, sph + 4);
  r = cap->radius + sph[8];
  return distSq <= r * r;
}
