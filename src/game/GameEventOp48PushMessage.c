// bdc 0x088edf04 GameEventOp48PushMessage
#include "bdc.h"

/* Handler of event opcode 0x48 (`GameEventExecCommand`): appends message id `arg` to the message
   queue `+0x50` (128 u16 entries, write index `+0x258`, wraps to 0). */

void GameEventOp48PushMessage(GameEvent *self, u8 flag, s16 arg)

{
  u32 next;
  
  next = self->msgCount + 1;
  self->msgIds[self->msgCount] = arg;
  self->msgCount = (u16)next;
  if (0x7f < (next & 0xffff)) {
    self->msgCount = 0;
  }
  return;
}

