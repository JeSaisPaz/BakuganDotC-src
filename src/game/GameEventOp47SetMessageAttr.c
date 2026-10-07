// bdc 0x088edee8 GameEventOp47SetMessageAttr
#include "bdc.h"

/* Handler of event opcode 0x47 (`GameEventExecCommand`): stores `arg` in the message attribute
   buffer `+0x150` at the current queue end `+0x258` (set before the matching
   `GameEventOp48PushMessage`). */

void GameEventOp47SetMessageAttr(GameEvent *self, s16 arg, s16 unused)

{
  self->msgAttrs[self->msgCount] = arg;
  return;
}

