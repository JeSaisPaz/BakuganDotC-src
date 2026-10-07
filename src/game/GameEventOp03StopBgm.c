// bdc 0x088ebce4 GameEventOp03StopBgm
#include "bdc.h"

/* Handler of event opcode 0x03 (`GameEventExecCommand`): cancels BGM channel 0 and fades it out
   over 0.4 s. */

void GameEventOp03StopBgm(GameEvent *self, u8 flag, s16 arg)

{
  SndBgmCancelChannel(0);
  SndBgmQueueStop(0.4f,0);
  return;
}

