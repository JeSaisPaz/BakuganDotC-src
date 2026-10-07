// bdc 0x088ed8f8 GameEventOp34PropMove2
#include "bdc.h"

/* Handler of event opcode 0x34 (`GameEventExecCommand`): same as `GameEventOp30PropMove`. */

void GameEventOp34PropMove2(GameEvent *self, u8 flag, s16 arg)

{
  GameEventAddPropTween(self,arg,'\0',flag);
  return;
}

