// bdc 0x088edea0 GameEventOp46PropNop
#include "bdc.h"

/* Handler of event opcode 0x46 (`GameEventExecCommand`): calls the empty `GameEventPropNop` for
   an existing prop of slot `flag`. */

void GameEventOp46PropNop(GameEvent *self, u8 flag, s16 arg)

{
  if (self->props[flag].prop != (void *)0x0) {
    GameEventPropNop();
  }
  return;
}

