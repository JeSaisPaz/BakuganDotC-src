// bdc 0x088f134c GameEventOp93StartMenuTask
#include "bdc.h"

/* Handler of event opcode 0x93 (`GameEvent470ExecCommand`): in skip mode only unblocks; otherwise
   runs `GameEvent470StartRewardScreen` with `value`. */

void GameEventOp93StartMenuTask(GameEvent *self, u16 value, s16 arg)
{
  if ((self->flags & 1) != 0) {
    self->blocking = 0;
    return;
  }
  GameEvent470StartRewardScreen((GameEvent470 *)self, value);
}
