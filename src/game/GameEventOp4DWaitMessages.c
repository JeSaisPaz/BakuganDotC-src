// bdc 0x088edf70 GameEventOp4DWaitMessages
#include "bdc.h"

/* Handler of event opcode 0x4d (`GameEventExecCommand`): blocks the script (wait type `+0x269 =
   2`) until every queued message has been shown (`GameEventUpdateMessageSequence`). */

void GameEventOp4DWaitMessages(GameEvent *self, u8 flag, s16 arg)

{
  self->waitType = '\x02';
  return;
}

