// bdc 0x088a8fe0 ActorStageObjHasGroundBelow
#include "bdc.h"

/* Casts a ray from 1000 units above `pos` along the cached down vector (`g_vecDown`, with its
   reciprocals) against the battle collision (`CollisionRaycast`, `g_collisionRayBlock`) and
   returns 1 when it hits, 0 otherwise or outside battle (`BtlCameraTaskExists` false). Used by
   `ActorStageObjCreateByKind` to skip props without ground and by
   `ActorStageObjCategoryNeedsGround`. */

int ActorStageObjHasGroundBelow(float *pos)
{
  g_collisionRayDesc.origin.x = pos[0];
  g_collisionRayDesc.origin.y = pos[1];
  g_collisionRayDesc.origin.z = pos[2];
  g_collisionRayDesc.origin.w = pos[3];
  g_collisionRayDesc.origin.y = g_collisionRayDesc.origin.y + 1000.0f;
  g_collisionRayDesc.dir.x = g_vecDown.x;
  g_collisionRayDesc.dir.y = g_vecDown.y;
  g_collisionRayDesc.dir.z = g_vecDown.z;
  g_collisionRayDesc.dir.w = g_vecDown.w;
  /* invDir.xyz = 1/dir.xyz, or the bank zero S713 where dir.i == 0 (vcmp EZ + vcmovt); w = S713. */
  g_collisionRayDesc.invDir.x = (g_collisionRayDesc.dir.x == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.x;
  g_collisionRayDesc.invDir.y = (g_collisionRayDesc.dir.y == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.y;
  g_collisionRayDesc.invDir.z = (g_collisionRayDesc.dir.z == 0.0f) ? 0.0f : 1.0f / g_collisionRayDesc.dir.z;
  g_collisionRayDesc.invDir.w = 0.0f;
  if (BtlCameraTaskExists() != 0 &&
      CollisionRaycast(0x3fbf2100, &g_collisionRayBlock, 0) != (void *)0) {
    return 1;
  }
  return 0;
}
