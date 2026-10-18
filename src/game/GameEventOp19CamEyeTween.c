// bdc 0x088ec694 GameEventOp19CamEyeTween
#include "bdc.h"

/* Handler of event opcode 0x19 (`GameEventExecCommand`): queues an eye tween of `frames` frames
   (the 16-bit command argument, second parameter) (`GameEventAddCamTween` kind 0). The third
   parameter is unused. */

void GameEventOp19CamEyeTween(GameEvent *self, u16 frames, s16 unused)

{
  GameEventAddCamTween(self, frames, 0);
  return;
}

