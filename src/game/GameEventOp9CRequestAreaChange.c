// bdc 0x088f14dc GameEventOp9CRequestAreaChange
#include "bdc.h"

/* Handler of event opcode 0x9c (`GameEvent470ExecCommand`): requests field transition kind 5 and
   sets the area `0x08b00bd4` to `flag`. */

void GameEventOp9CRequestAreaChange(GameEvent *self, u8 flag, s16 arg)

{
  if ((self->flags & 1) != 0) {
    self->flags = self->flags & 0xfb;
  }
  g_gameEventTransitionKind = 5;
  g_gameEventFlags[0] = flag;
  return;
}

