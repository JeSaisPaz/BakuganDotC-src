// bdc 0x088f1414 GameEventOp99RequestTransition2
#include "bdc.h"

/* Handler of event opcode 0x99 (`GameEvent470ExecCommand`): requests field transition kind 2
   (`0x08b00dc4 = 2`). */

void GameEventOp99RequestTransition2(GameEvent *self, u8 flag, s16 arg)

{
  if ((self->flags & 1) != 0) {
    self->flags = self->flags & 0xfb;
  }
  g_gameEventTransitionKind = 2;
  return;
}

