// bdc 0x088ed054 GameEventOp21CamTargetTween
#include "bdc.h"

/* Handler of event opcode 0x21 (`GameEventExecCommand`): queues a target tween of `flag` frames (second argument, a1)
   (kind 1). */

void GameEventOp21CamTargetTween(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddCamTween(self,flag,1);
  return;
}

