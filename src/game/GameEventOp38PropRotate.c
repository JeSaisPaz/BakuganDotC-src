// bdc 0x088eda48 GameEventOp38PropRotate
#include "bdc.h"

/* Handler of event opcode 0x38 (`GameEventExecCommand`): queues a rotate tween of `arg` frames
   for prop slot `flag` (kind 1). */

void GameEventOp38PropRotate(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddPropTween(self,arg,'\x01',flag);
  return;
}

