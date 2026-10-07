// bdc 0x089e8fe0 CollisionSphereOverlapCapsule
#include "bdc.h"

/* Returns whether a sphere (centre, radius) and a capsule (axis segment, radius) overlap: the
   squared distance from the sphere centre to the segment (`SegmentDistSqToPoint`) is at most the
   squared sum of the radii. The segment record starts 0x10 bytes into the capsule, so its `start`
   is the capsule's `start`. */

bool CollisionSphereOverlapCapsule(const CollisionSphere *sphere, const CollisionCapsule *capsule)
{
  float distSq;
  float r;

  distSq = SegmentDistSqToPoint(
      (SegmentShape *)((u8 *)capsule->start - __builtin_offsetof(SegmentShape, start)),
      sphere->center);
  r = sphere->radius + capsule->radius;
  return distSq <= r * r;
}
