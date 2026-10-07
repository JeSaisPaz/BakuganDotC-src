// bdc 0x088f1868 GameEventOpA8End2
#include "bdc.h"

/* Handler of event opcode 0xa8 (`GameEvent470ExecCommand`): ends the script (command index =
   count), like `GameEventOp92End`. */

void GameEventOpA8End2(GameEvent *self, u8 flag, s16 arg)

{
  self->cmdIndex = self->cmdEnd;
  return;
}

