// bdc 0x0881a8d0 CollisionRaycastRay
#include "bdc.h"

/* Casts the ray `origin` + t·`dir` (vec4s copied into g_collisionRayDesc.origin/.dir) against the
   world colliders in `layerMask` with `CollisionRaycast` (query g_collisionRayBlock) and returns
   its result (the hit collider or NULL; hit point in g_collisionHitResult). Used by
   `GameFieldCameraModeOrbit`. */

void * CollisionRaycastRay(u32 layerMask, const float *origin, const float *dir)

{
  g_collisionRayDesc.origin.x = origin[0];
  g_collisionRayDesc.origin.y = origin[1];
  g_collisionRayDesc.origin.z = origin[2];
  g_collisionRayDesc.origin.w = origin[3];
  g_collisionRayDesc.dir.x = dir[0];
  g_collisionRayDesc.dir.y = dir[1];
  g_collisionRayDesc.dir.z = dir[2];
  g_collisionRayDesc.dir.w = dir[3];
  return CollisionRaycast(layerMask,&g_collisionRayBlock,0);
}
