// bdc 0x088edd68 GameEventOp3FPropScale
#include "bdc.h"

/* Handler of event opcode 0x3f (`GameEventExecCommand`): queues a scale tween of `arg` frames for
   prop slot `flag` (kind 2). */

void GameEventOp3FPropScale(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddPropTween(self,arg,'\x02',flag);
  return;
}

