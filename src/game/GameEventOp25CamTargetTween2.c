// bdc 0x088ed1e8 GameEventOp25CamTargetTween2
#include "bdc.h"

/* Handler of event opcode 0x25 (`GameEventExecCommand`): same as `GameEventOp21CamTargetTween`.
    */

void GameEventOp25CamTargetTween2(GameEvent *self, u16 frames, s16 unused)

{
  GameEventAddCamTween(self,frames,1);
  return;
}

