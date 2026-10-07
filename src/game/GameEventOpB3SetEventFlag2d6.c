// bdc 0x088f1aa0 GameEventOpB3SetEventFlag2d6
#include "bdc.h"

/* Handler of event opcode 0xb3 (`GameEvent470ExecCommand`): sets bit 0 of `+0x2d6` from `flag`.
    */

void GameEventOpB3SetEventFlag2d6(GameEvent470 *self, u8 flag, s16 arg)

{
  self->flags470 = self->flags470 & 0xfe | flag & 1;
  return;
}

