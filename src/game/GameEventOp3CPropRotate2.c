// bdc 0x088edbbc GameEventOp3CPropRotate2
#include "bdc.h"

/* Handler of event opcode 0x3c (`GameEventExecCommand`): same as `GameEventOp38PropRotate`. */

void GameEventOp3CPropRotate2(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddPropTween(self,arg,'\x01',flag);
  return;
}

