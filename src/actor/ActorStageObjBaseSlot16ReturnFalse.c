// bdc 0x08a2bfd0 ActorStageObjBaseSlot16ReturnFalse
#include "bdc.h"

/* Base stage-object class test, vtable slot 16 (`+0x84`): returns 0; only the mine
   (`ActorStageObjMineSlot16ReturnTrue`) and the wind generator
   (`ActorStageObjWindGeneratorSlot16ReturnTrue`) return 1. */

int ActorStageObjBaseSlot16ReturnFalse(ActorStageObjBase *self)

{
  return 0;
}

