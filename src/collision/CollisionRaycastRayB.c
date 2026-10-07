// bdc 0x0881a914 CollisionRaycastRayB
#include "bdc.h"

/* Same as `CollisionRaycastRay` but with its own static query block (g_collisionSegmentDesc
   .start/.dir, query g_collisionSegmentBlock): casts `origin` + t·`dir` against the colliders in
   `layerMask` and returns the hit collider or NULL. Used by actor code
   (`ActorSeparateFromActors`, `ActorNpcCanSeePlayer`, …). */

void * CollisionRaycastRayB(u32 layerMask, const float *origin, const float *dir)

{
  g_collisionSegmentDesc.start[0] = origin[0];
  g_collisionSegmentDesc.start[1] = origin[1];
  g_collisionSegmentDesc.start[2] = origin[2];
  g_collisionSegmentDesc.start[3] = origin[3];
  g_collisionSegmentDesc.dir[0] = dir[0];
  g_collisionSegmentDesc.dir[1] = dir[1];
  g_collisionSegmentDesc.dir[2] = dir[2];
  g_collisionSegmentDesc.dir[3] = dir[3];
  return CollisionRaycast(layerMask,&g_collisionSegmentBlock,0);
}
