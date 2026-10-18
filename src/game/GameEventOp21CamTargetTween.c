// bdc 0x088ed054 GameEventOp21CamTargetTween
#include "bdc.h"

/* Handler of event opcode 0x21 (`GameEventExecCommand`): queues a target tween (kind 1) of
   `frames` frames (the command's 16-bit argument, passed as `(u16)arg`). `unused` is ignored. */

void GameEventOp21CamTargetTween(GameEvent *self, u16 frames, s16 unused)

{
  GameEventAddCamTween(self,frames,1);
  return;
}

