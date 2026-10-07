// bdc 0x088edfe0 GameEventOp52WaitMessagesAuto
#include "bdc.h"

/* Handler of event opcode 0x52 (`GameEventExecCommand`): like `GameEventOp4DWaitMessages` but
   with an auto-advance time `flag` (`+0x25c`/`+0x25e`) for each message window. */

void GameEventOp52WaitMessagesAuto(GameEvent *self, s16 flag, s16 arg)

{
  self->msgAutoTime = flag;
  self->msgAutoTimer = flag;
  self->waitType = 2;
}
