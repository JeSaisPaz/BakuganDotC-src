// bdc 0x08a2c1b0 GameGimmickGetCollisionShape
#include "bdc.h"

/* Gimmick vtable entry 19 (`+0x9c`), root implementation: the gimmick has no collision shape
   record, returns NULL. */

void *GameGimmickGetCollisionShape(GameGimmick *gimmick)

{
  return (void *)0x0;
}

