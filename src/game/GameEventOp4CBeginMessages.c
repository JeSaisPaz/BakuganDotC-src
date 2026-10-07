// bdc 0x088edf54 GameEventOp4CBeginMessages
#include "bdc.h"

/* Handler of event opcode 0x4c (`GameEventExecCommand`): starts a new message block: resets the
   sequence state `+0x26a`, moves the read position `+0x256` to the queue end `+0x258` (messages
   pushed afterwards with `GameEventOp48PushMessage` form the block) and sets a zero-frame wait
   (`+0x260 = 0`, `+0x269 = 1`). */

void GameEventOp4CBeginMessages(GameEvent *self, u8 flag, s16 arg)

{
  self->msgState = '\0';
  self->msgPos = self->msgCount;
  self->waitFrames = 0;
  self->waitType = '\x01';
  return;
}

