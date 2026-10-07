// bdc 0x088b78e0 ActorBallTrailCtor
#include "bdc.h"

/* Constructor of the thrown-ball trail (0x840 bytes, owned by the player actor at `+0x4a4`): clears
   `ball`, sets `segIndex = -1`, clears `enable`, zeroes `lastPos` (the
   VFPU bank constant C720 = (0, 0, 0, 0)), allocates the 192-entry sprite table `sprites` (low
   heap, 0x300 bytes, zeroed) and fills it with billboards of the texture `"eff_ball_a"`
   (`GfxSpriteLayerCreateBillboardByName` on `g_billboardSpriteLayer`; blend 2, size 0.2,
   alpha 0, parked at (0, -1000, 0, 0)), then `ActorBallTrailReset``(trail, 1)`. Always returns
   `trail`. */

ActorBallTrail *ActorBallTrailCtor(ActorBallTrail *trail)
{
  float park[4];
  bool fromLow;
  void **table;
  GfxSprite *sprite;
  int i;

  trail->ball = NULL;
  trail->segIndex = -1;
  trail->enable = 0;
  trail->lastPos[0] = 0.0f;
  trail->lastPos[1] = 0.0f;
  trail->lastPos[2] = 0.0f;
  trail->lastPos[3] = 0.0f;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  table = (void **)MemAlloc(0x300, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  trail->sprites = table;
  memset(table, 0, 0x300);
  for (i = 0; i < 0xc0; i++) {
    sprite = GfxSpriteLayerCreateBillboardByName(g_billboardSpriteLayer, "eff_ball_a");
    sprite->blendMode = 2;
    park[0] = 0.0f;
    park[1] = -1000.0f;
    park[2] = 0.0f;
    park[3] = 0.0f;
    sprite->posX = park[0];
    sprite->posY = park[1];
    sprite->posZ = park[2];
    sprite->posW = park[3];
    sprite->alpha = 0.0f;
    sprite->width = 0.2f;
    sprite->height = 0.2f;
    sprite->depth = 0.2f;
    sprite->maybe_sizeW = 0.0f;
    trail->sprites[i] = sprite;
  }
  ActorBallTrailReset(trail, true);
  return trail;
}
