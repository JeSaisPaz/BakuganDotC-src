// bdc 0x088f0f84 GameEventOp7FSetMotionNoLoop
#include "bdc.h"

/* Handler of event opcode 0x7f (`GameEvent470ExecCommand`): sets bit 2 of `+0x2d6` from `arg` (1
   = the next actor motion does not loop). */

void GameEventOp7FSetMotionNoLoop(GameEvent470 *self, u8 flag, s16 arg)

{
  self->flags470 = self->flags470 & 0xfb | (flag & 1) << 2;
  return;
}

