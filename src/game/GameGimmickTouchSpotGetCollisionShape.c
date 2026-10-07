// bdc 0x08a2c3e8 GameGimmickTouchSpotGetCollisionShape
#include "bdc.h"

/* Vtable `0x08af3684` entry 19 (`+0x9c`) of the touch-spot gimmick (`GameGimmickTouchSpotCtor`):
   returns its collision shape record `gimmick + 0x190` (type 3 sphere, vtable `0x08af55c4` at
   `+0x194`, centre `+0x1a0`, radius 3 at `+0x1b0`), the one passed to its collider. */

void *GameGimmickTouchSpotGetCollisionShape(GameGimmickTouchSpot *gimmick)

{
  return &gimmick->shapeType;
}

