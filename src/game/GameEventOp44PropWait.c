// bdc 0x088ede58 GameEventOp44PropWait
#include "bdc.h"

/* Handler of event opcode 0x44 (`GameEventExecCommand`): queues a kind-3 prop action of `arg`
   frames for slot `flag` (always queued, even when skipping). */

void GameEventOp44PropWait(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddPropTween(self,arg,'\x03',flag);
  return;
}

