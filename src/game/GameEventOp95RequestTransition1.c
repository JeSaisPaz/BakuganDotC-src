// bdc 0x088f1390 GameEventOp95RequestTransition1
#include "bdc.h"

/* Handler of event opcode 0x95 (`GameEvent470ExecCommand`): leaves skip-fade mode (clears
   `+0x273` bit 2 when skipping) and requests field transition kind 1 (`0x08b00dc4 = 1`) with
   parameter `value` (second argument, `0x08b00dc0`); `arg` is unused. */

void GameEventOp95RequestTransition1(GameEvent *self, s16 value, s16 arg)

{
  if ((self->flags & 1) != 0) {
    self->flags = self->flags & 0xfb;
  }
  g_gameEventTransitionKind = 1;
  g_gameEventTransitionArg = value;
  return;
}

