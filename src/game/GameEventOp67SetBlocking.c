// bdc 0x088ee09c GameEventOp67SetBlocking
#include "bdc.h"

/* Handler of event opcode 0x67 (`GameEventExecCommand`): stores `flag` in `+0x272`;
   `GameEventRunCommands` stops executing commands for this frame when it is cleared. */

void GameEventOp67SetBlocking(GameEvent *self, u8 flag, s16 arg)

{
  self->blocking = flag;
  return;
}

