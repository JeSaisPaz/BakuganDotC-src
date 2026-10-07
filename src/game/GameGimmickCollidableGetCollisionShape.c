// bdc 0x08a2c194 GameGimmickCollidableGetCollisionShape
#include "bdc.h"

/* Vtable `0x08af2e2c` entry 19 (`+0x9c`) of the collidable gimmick (`GameGimmickCollidableCtor`):
   returns its collision shape record `gimmick + 0x190` (type word, sphere shape vtable `0x08af55c4`
   at `+0x194`). */

void *GameGimmickCollidableGetCollisionShape(GameGimmickCollidable *gimmick)

{
  return &gimmick->shapeType;
}

