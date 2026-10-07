// bdc 0x088edd9c GameEventOp41SetPropMotionOnce
#include "bdc.h"

/* Handler of event opcode 0x41 (`GameEventExecCommand`): copies bit 0 of `flag` into bit 3 of
   `flags` (`+0x273`; 1 = play the next prop motion without looping). `arg` is unused. */

void GameEventOp41SetPropMotionOnce(GameEvent *self, u8 flag, s16 arg)

{
  self->flags = self->flags & 0xf7 | (flag & 1) << 3;
  return;
}

