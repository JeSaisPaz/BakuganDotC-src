// bdc 0x088b9410 ActorBallCreate
#include "bdc.h"

/* Allocates (low heap, 0x200 bytes) and constructs an `ActorBall` of `kind` (`ActorBallCtor`),
   multiplies its scale by `scale`, optionally places it at `pos` (`CollisionRaycastPoint`,
   heading `pos[3]` via `ActorBallSetHeading`) and sets its ambient/diffuse lighting (brighter,
   with specular 0.3, when `GameStageIs0Or13` is non-zero). Returns the object, or NULL when
   the allocation failed. `scale[3]` is set to 0 (the VFPU bank zero S713 stored with the scaled
   `scale.xyz`). */

CoreObject *ActorBallCreate(float scale, u32 kind, s32 unused, float *pos)

{
  bool fromLow;
  ActorBall *mem;
  ActorBall *ball;
  float specular[4];

  (void)unused;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(0x200, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  ball = NULL;
  if (mem != NULL) {
    ActorBallCtor(&mem->base.base, kind);
    ball = mem;
  }
  if (ball != NULL) {
    ball->base.scale[0] = ball->base.scale[0] * scale;
    ball->base.scale[1] = ball->base.scale[1] * scale;
    ball->base.scale[2] = ball->base.scale[2] * scale;
    ball->base.scale[3] = 0.0f; /* `sv.q` of C710: lane 3 is the bank zero S713 */
    if (pos != NULL) {
      CollisionRaycastPoint(pos, ball->base.pos);
      ActorBallSetHeading(pos[3], &ball->base);
    }
    if (GameStageIs0Or13() != 0) {
      ball->base.color[0] = 0.45f;
      ball->base.color[1] = 0.45f;
      ball->base.color[2] = 0.55f;
      ball->base.color[3] = 1.0f;
      ball->base.ambient[0] = 0.4f;
      ball->base.ambient[1] = 0.4f;
      ball->base.ambient[2] = 0.4f;
      ball->base.ambient[3] = 1.0f;
      specular[0] = 0.3f;
      specular[1] = 0.3f;
      specular[2] = 0.3f;
      specular[3] = 1.0f;
      GfxModelSetSpecular(10.0f, &ball->base, specular, NULL);
    }
    else {
      ball->base.ambient[3] = 1.0f;
      ball->base.ambient[0] = 0.7f;
      ball->base.ambient[1] = 0.7f;
      ball->base.ambient[2] = 0.7f;
    }
  }
  return &ball->base.base;
}
