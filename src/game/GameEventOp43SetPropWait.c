// bdc 0x088ede20 GameEventOp43SetPropWait
#include "bdc.h"

/* Handler of event opcode 0x43 (`GameEventExecCommand`): copies the record's `+4` into `+0x48`
   and stores the wait length `arg` in `+0x44` of prop slot `flag`. */

void GameEventOp43SetPropWait(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].bobBaseY = self->props[flag].pos[1];
  self->props[flag].bobAmp = (uint)(ushort)arg;
  return;
}

