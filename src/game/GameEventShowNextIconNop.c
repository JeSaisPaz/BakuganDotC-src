// bdc 0x08a2c6a8 GameEventShowNextIconNop
#include "bdc.h"

/* Entry 9 (`+0x4c`) of the field event task base vtable `0x08af41ec`: empty; task 470 overrides it
   with `GameEvent470ShowNextIcon`. */

void GameEventShowNextIconNop(GameEvent *self)

{
  return;
}

