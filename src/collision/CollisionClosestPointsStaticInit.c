// bdc 0x089ea8ac CollisionClosestPointsStaticInit
#include "bdc.h"

/* Static constructor of the closest-point scratch vectors `0x08b025e0` and `0x08b025f0` (written by
   `CollisionRaySegmentDistSq` / `CollisionSegmentSegmentDistSq`): zeroes both
   `ScePspFVector4`s. */

void CollisionClosestPointsStaticInit(void)

{
  g_collisionClosestPoints[0].x = 0.0;
  g_collisionClosestPoints[0].y = 0.0;
  g_collisionClosestPoints[0].z = 0.0;
  g_collisionClosestPoints[0].w = 0.0;
  g_collisionClosestPoints[1].x = 0.0;
  g_collisionClosestPoints[1].y = 0.0;
  g_collisionClosestPoints[1].z = 0.0;
  g_collisionClosestPoints[1].w = 0.0;
  return;
}

