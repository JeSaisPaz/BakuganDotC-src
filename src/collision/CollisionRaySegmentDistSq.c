// bdc 0x089e87f4 CollisionRaySegmentDistSq
#include "bdc.h"

/* Squared distance between a ray and a segment (`CollisionRaySegmentClosest`), leaving the two
   parameters in `0x08ac5d68`/`0x08ac5d6c` and the closest points in `0x08b025e0`/`0x08b025f0`. */

float CollisionRaySegmentDistSq(const void *ray, const void *seg)

{
  return CollisionRaySegmentClosest(ray, seg, &g_collisionClosestParams[0],
                                    &g_collisionClosestParams[1], &g_collisionClosestPoints[0],
                                    &g_collisionClosestPoints[1]);
}
