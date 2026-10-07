// bdc 0x0889d53c CollisionRaycastPoint
#include "bdc.h"

/* Casts a ray from the point `pos` (vec4) along g_vecDown against the world colliders with layer
   mask 0x3fbf2700 (`CollisionRaycast` on g_collisionRayBlock). On a hit it copies the hit point
   g_collisionHitResult.point to `out` and returns 1; on a miss it copies `pos` to `out` and
   returns 0. The ray descriptor g_collisionRayDesc gets origin = `pos`, dir = g_vecDown and
   invDir = (1/dir.x, 1/dir.y, 1/dir.z, 0), a lane whose dir component is 0 taking 0 (bank S713). */

s32 CollisionRaycastPoint(float *pos, float *out)

{
  void *hit;

  g_collisionRayDesc.origin.x = pos[0];
  g_collisionRayDesc.origin.y = pos[1];
  g_collisionRayDesc.origin.z = pos[2];
  g_collisionRayDesc.origin.w = pos[3];
  g_collisionRayDesc.dir = g_vecDown;
  /* vrcp per lane, replaced by the bank zero S713 where the lane is 0; lane w is S713 too. */
  g_collisionRayDesc.invDir.x =
      (g_collisionRayDesc.dir.x == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.x;
  g_collisionRayDesc.invDir.y =
      (g_collisionRayDesc.dir.y == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.y;
  g_collisionRayDesc.invDir.z =
      (g_collisionRayDesc.dir.z == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.z;
  g_collisionRayDesc.invDir.w = 0.0f;
  hit = CollisionRaycast(0x3fbf2700, &g_collisionRayBlock, 0);
  if (hit != (void *)0) {
    out[0] = g_collisionHitResult.point.x;
    out[1] = g_collisionHitResult.point.y;
    out[2] = g_collisionHitResult.point.z;
    out[3] = g_collisionHitResult.point.w;
    return 1;
  }
  out[0] = pos[0];
  out[1] = pos[1];
  out[2] = pos[2];
  out[3] = pos[3];
  return 0;
}
