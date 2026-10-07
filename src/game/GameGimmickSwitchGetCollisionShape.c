// bdc 0x08a2c3f8 GameGimmickSwitchGetCollisionShape
#include "bdc.h"

/* Vtable `0x08af3734` entry 19 (`+0x9c`) of the floor-switch gimmick (`GameGimmickSwitchCtor`):
   returns its collision shape record `gimmick + 0x1a0` (type 3, vtable at `+0x1a4`). */

void *GameGimmickSwitchGetCollisionShape(GameGimmickSwitch *gimmick)

{
  return &gimmick->shapeType;
}

