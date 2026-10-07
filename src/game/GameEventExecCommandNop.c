// bdc 0x08a2c6b8 GameEventExecCommandNop
#include "bdc.h"

/* Entry 11 (`+0x5c`) of the field event task base vtable `0x08af41ec`: empty; task 470 overrides it
   with `GameEvent470ExecCommand`. */

void GameEventExecCommandNop(GameEvent *self, u8 op, u8 flag, s16 arg)

{
  return;
}

