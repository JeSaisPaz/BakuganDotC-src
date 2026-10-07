// bdc 0x089e96c8 CollisionCapsuleVsCapsule
#include "bdc.h"

/* Capsule shape method for capsule queries (type-4 slot 3): segment/segment closest points; hit
   when distSq <= (radiusA + radiusB)², `out` = midpoint of the two closest points (all four lanes). */

bool CollisionCapsuleVsCapsule(const void *a, const void *b, ScePspFVector4 *out)

{
  const CollisionCapsule *capA = a;
  const CollisionCapsule *capB = b;
  float distSq;
  float r;
  ScePspFVector4 p0;
  ScePspFVector4 p1;

  distSq = CollisionSegmentSegmentDistSq(&capA->segmentHead, &capB->segmentHead);
  r = capA->radius + capB->radius;
  p0 = g_collisionClosestPoints[0];
  p1 = g_collisionClosestPoints[1];
  out->x = p0.x + (p1.x - p0.x) * 0.5f;
  out->y = p0.y + (p1.y - p0.y) * 0.5f;
  out->z = p0.z + (p1.z - p0.z) * 0.5f;
  out->w = p0.w + (p1.w - p0.w) * 0.5f;
  return distSq <= r * r;
}
