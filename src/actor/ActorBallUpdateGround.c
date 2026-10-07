// bdc 0x088b8590 ActorBallUpdateGround
#include "bdc.h"

/* Ground probe of an `ActorBall` (`ActorBallCtor`), skipped when `noGroundProbe` is set: puts the
   world position of model node 1 (`GfxModelGetNodeWorldPosByIndex`) and the global down vector
   `g_vecDown` into the ray query `g_collisionRayDesc` (inverse direction per xyz component, 0 for
   a zero component, w 0) and casts it (`CollisionRaycast`, mask 0x3fbf2700). On a hit stores the
   hit point in `groundPoint` and moves `groundNormal` half-way toward the hit normal, then
   renormalises its xyz (each clamped to [-1, 1], a zero length giving 0) and clears its w; without
   a hit only `groundPoint.x/z` follow the node. */

void ActorBallUpdateGround(CoreObject *obj)
{
  ActorBall *ball = (ActorBall *)obj;
  ScePspFVector4 nodePos BDC_ALIGN16;
  ScePspFVector4 dir;
  float n[4];
  float s;
  int i;

  if (ball->noGroundProbe != 0) {
    return;
  }
  GfxModelGetNodeWorldPosByIndex(&ball->base, &nodePos, 1);
  g_collisionRayDesc.origin = nodePos;
  g_collisionRayDesc.dir = g_vecDown;
  dir = g_collisionRayDesc.dir;
  g_collisionRayDesc.invDir.x = dir.x == 0.0f ? 0.0f : VfRcp(dir.x);
  g_collisionRayDesc.invDir.y = dir.y == 0.0f ? 0.0f : VfRcp(dir.y);
  g_collisionRayDesc.invDir.z = dir.z == 0.0f ? 0.0f : VfRcp(dir.z);
  g_collisionRayDesc.invDir.w = 0.0f;
  if (CollisionRaycast(0x3fbf2700, &g_collisionRayBlock, 0) != NULL) {
    ball->groundPoint[0] = g_collisionHitResult.point.x;
    ball->groundPoint[1] = g_collisionHitResult.point.y;
    ball->groundPoint[2] = g_collisionHitResult.point.z;
    ball->groundPoint[3] = g_collisionHitResult.point.w;
    n[0] = ball->groundNormal[0] + (g_collisionHitResult.normal.x - ball->groundNormal[0]) * 0.5f;
    n[1] = ball->groundNormal[1] + (g_collisionHitResult.normal.y - ball->groundNormal[1]) * 0.5f;
    n[2] = ball->groundNormal[2] + (g_collisionHitResult.normal.z - ball->groundNormal[2]) * 0.5f;
    n[3] = ball->groundNormal[3] + (g_collisionHitResult.normal.w - ball->groundNormal[3]) * 0.5f;
    s = n[0] * n[0] + n[1] * n[1] + n[2] * n[2];
    s = s == 0.0f ? 0.0f : VfRsq(s);
    for (i = 0; i < 3; i++) {
      ball->groundNormal[i] = VfSat1(n[i] * s);
    }
    ball->groundNormal[3] = 0.0f;
  }
  else {
    ball->groundPoint[0] = g_collisionRayDesc.origin.x;
    ball->groundPoint[2] = g_collisionRayDesc.origin.z;
  }
}
