// bdc 0x089e93e8 CollisionCapsuleOverlapCapsule
#include "bdc.h"

/* Returns whether two capsules overlap (`CollisionSegmentSegmentDistSq` against the summed
   radii). */

bool CollisionCapsuleOverlapCapsule(const void *a, const void *b)

{
  const CollisionCapsule *capA = a;
  const CollisionCapsule *capB = b;
  float distSq;
  float r;

  distSq = CollisionSegmentSegmentDistSq(&capA->segmentHead, &capB->segmentHead);
  r = capA->radius + capB->radius;
  return distSq <= r * r;
}
