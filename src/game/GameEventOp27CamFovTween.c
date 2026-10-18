// bdc 0x088ed218 GameEventOp27CamFovTween
#include "bdc.h"

/* Handler of event opcode 0x27 (`GameEventExecCommand`): queues a field-of-view tween of `frames`
   frames (kind 2; the 16-bit command argument, `(u16)arg`). */

void GameEventOp27CamFovTween(GameEvent *self, u16 frames, s16 unused)

{
  GameEventAddCamTween(self,frames,2);
  return;
}

