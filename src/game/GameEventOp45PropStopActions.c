// bdc 0x088ede7c GameEventOp45PropStopActions
#include "bdc.h"

/* Handler of event opcode 0x45 (`GameEventExecCommand`): removes the queued prop actions (kind 2)
   of slot `flag` (`GameEventActionListRemoveMatching`). */

void GameEventOp45PropStopActions(GameEvent *self, u8 flag, s16 arg)

{
  GameEventActionListRemoveMatching(self->actions,2,flag);
  return;
}

