// bdc 0x088ecefc GameEventOp1DCamEyeTween2
#include "bdc.h"

/* Handler of event opcode 0x1d (`GameEventExecCommand`): same as `GameEventOp19CamEyeTween`. */

void GameEventOp1DCamEyeTween2(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddCamTween(self,flag,0);
  return;
}

