// bdc 0x08a2c004 ActorStageObjMineSlot16ReturnTrue
#include "bdc.h"

/* Mine override of the stage-object class test slot 16 (`+0x84`): returns 1. */

int ActorStageObjMineSlot16ReturnTrue(ActorStageObjMine *self)

{
  return 1;
}

