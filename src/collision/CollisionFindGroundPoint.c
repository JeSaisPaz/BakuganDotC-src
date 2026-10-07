// bdc 0x088a8eec CollisionFindGroundPoint
#include "bdc.h"

/* Finds the ground below a point: casts a ray straight down from `pos` raised by 200.0 against the
   colliders selected by `layerMask` (`CollisionRaycast`) and writes the hit point (4 floats) to
   `out`; when nothing is hit `out` is `pos` with its Y component forced to 0. */

void CollisionFindGroundPoint(float *out, const float *pos, u32 layerMask)

{
  void *hit;

  out[2] = 0.0f;
  out[1] = 0.0f;
  out[0] = 0.0f;
  out[3] = 0.0f;
  g_collisionRayDesc.origin.x = pos[0];
  g_collisionRayDesc.origin.y = pos[1];
  g_collisionRayDesc.origin.z = pos[2];
  g_collisionRayDesc.origin.w = pos[3];
  g_collisionRayDesc.origin.y = g_collisionRayDesc.origin.y + 200.0f;
  g_collisionRayDesc.dir.x = g_vecDown.x;
  g_collisionRayDesc.dir.y = g_vecDown.y;
  g_collisionRayDesc.dir.z = g_vecDown.z;
  g_collisionRayDesc.dir.w = g_vecDown.w;
  /* invDir.xyz = 1/dir.xyz, 0 (bank S713) where dir.i == 0; invDir.w = bank S713 = 0. */
  g_collisionRayDesc.invDir.x = (g_collisionRayDesc.dir.x == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.x;
  g_collisionRayDesc.invDir.y = (g_collisionRayDesc.dir.y == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.y;
  g_collisionRayDesc.invDir.z = (g_collisionRayDesc.dir.z == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.z;
  g_collisionRayDesc.invDir.w = 0.0f;
  hit = CollisionRaycast(layerMask, &g_collisionRayBlock, 0);
  if (hit != (void *)0) {
    out[0] = g_collisionHitResult.point.x;
    out[1] = g_collisionHitResult.point.y;
    out[2] = g_collisionHitResult.point.z;
    out[3] = g_collisionHitResult.point.w;
  } else {
    out[0] = pos[0];
    out[1] = pos[1];
    out[2] = pos[2];
    out[3] = pos[3];
    out[1] = 0.0f;
  }
}
