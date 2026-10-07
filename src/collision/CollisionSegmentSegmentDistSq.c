// bdc 0x089e882c CollisionSegmentSegmentDistSq
#include "bdc.h"

/* Squared distance between two segments (`CollisionSegmentSegmentClosest`), leaving the
   parameters in `g_collisionClosestParams` and the closest points in `g_collisionClosestPoints`.
   Used by the capsule tests and `BtlAttackCheckClash`. Returns the squared distance. */

float CollisionSegmentSegmentDistSq(const void *a, const void *b)
{
  return CollisionSegmentSegmentClosest(a, b, &g_collisionClosestParams[0], &g_collisionClosestParams[1],
                                        &g_collisionClosestPoints[0], &g_collisionClosestPoints[1]);
}
