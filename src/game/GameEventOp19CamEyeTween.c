// bdc 0x088ec694 GameEventOp19CamEyeTween
#include "bdc.h"

/* Handler of event opcode 0x19 (`GameEventExecCommand`): queues an eye tween of `flag` frames (second argument, a1)
   (`GameEventAddCamTween` kind 0). */

void GameEventOp19CamEyeTween(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddCamTween(self,flag,0);
  return;
}

