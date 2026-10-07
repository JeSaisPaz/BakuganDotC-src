// bdc 0x08a2c6b0 GameEventSkipLabelsNop
#include "bdc.h"

/* Entry 10 (`+0x54`) of the field event task base vtable `0x08af41ec`: empty; task 470 overrides it
   with `GameEvent470SkipLabels`. */

void GameEventSkipLabelsNop(GameEvent *self, s32 count)

{
  return;
}

