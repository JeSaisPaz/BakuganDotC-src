// bdc 0x088f12c8 GameEventOp8FSetTestFlag
#include "bdc.h"

/* Handler of event opcode 0x8f (`GameEvent470ExecCommand`): selects the story flag `+0x28c`
   tested by `GameEventOp90IfFlagSkip`. */

void GameEventOp8FSetTestFlag(GameEvent470 *self, u16 id, s16 arg)

{
  self->testFlag = id;
  return;
}

