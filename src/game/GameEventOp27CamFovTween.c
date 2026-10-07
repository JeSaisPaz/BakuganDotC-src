// bdc 0x088ed218 GameEventOp27CamFovTween
#include "bdc.h"

/* Handler of event opcode 0x27 (`GameEventExecCommand`): queues a field-of-view tween of `arg`
   frames (kind 2). */

void GameEventOp27CamFovTween(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddCamTween(self,flag,2);
  return;
}

