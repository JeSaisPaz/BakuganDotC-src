// bdc 0x088ecefc GameEventOp1DCamEyeTween2
#include "bdc.h"

/* Handler of event opcode 0x1d (`GameEventExecCommand`): same as `GameEventOp19CamEyeTween`:
   queues an eye tween of `frames` frames (the 16-bit command argument, second parameter)
   (`GameEventAddCamTween` kind 0). The third parameter is unused. */

void GameEventOp1DCamEyeTween2(GameEvent *self, u16 frames, s16 unused)

{
  GameEventAddCamTween(self, frames, 0);
  return;
}

