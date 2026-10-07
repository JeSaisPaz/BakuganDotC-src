// bdc 0x088ed700 GameEventOp30PropMove
#include "bdc.h"

/* Handler of event opcode 0x30 (`GameEventExecCommand`): queues a move tween of `arg` frames for
   prop slot `flag` (`GameEventAddPropTween` kind 0). */

void GameEventOp30PropMove(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddPropTween(self,arg,'\0',flag);
  return;
}

