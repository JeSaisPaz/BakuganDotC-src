// bdc 0x088f148c GameEventOp9BRequestTransition4
#include "bdc.h"

/* Handler of event opcode 0x9b (`GameEvent470ExecCommand`): requests field transition kind 4 and
   sets bit 1 of `g_gameEventTransitionFlags` from `flag`. */

void GameEventOp9BRequestTransition4(GameEvent *self, u8 flag, s16 arg)

{
  if ((self->flags & 1) != 0) {
    self->flags = self->flags & 0xfb;
  }
  g_gameEventTransitionKind = '\x04';
  g_gameEventTransitionFlags = g_gameEventTransitionFlags & 0xfd | (flag & 1) << 1;
  return;
}

