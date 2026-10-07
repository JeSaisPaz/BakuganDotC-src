// bdc 0x08a2bfa0 GameGimmickCorePointGetCollisionShape
#include "bdc.h"

/* Vtable slot 19 of the core-point gimmick (`GameGimmickCorePointCtor`, vtable `0x08af234c`):
   returns its collision shape record, `gimmick + 0x1b0`. */

void *GameGimmickCorePointGetCollisionShape(GameGimmickCorePoint *gimmick)

{
  return &gimmick->shapeType;
}

