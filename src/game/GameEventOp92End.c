// bdc 0x088f1340 GameEventOp92End
#include "bdc.h"

/* Handler of event opcode 0x92 (`GameEvent470ExecCommand`): ends the script (command index =
   count). */

void GameEventOp92End(GameEvent *self, u8 flag, s16 arg)

{
  self->cmdIndex = self->cmdEnd;
  return;
}

