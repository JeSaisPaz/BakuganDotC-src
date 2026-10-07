// bdc 0x08a2c6c0 GameEventCheckWaitReturnTrue
#include "bdc.h"

/* Entry 12 (`+0x64`) of the field event task base vtable `0x08af41ec`: returns 1 (never waits);
   task 470 overrides it with `GameEvent470CheckWait`. */

s32 GameEventCheckWaitReturnTrue(GameEvent *self)

{
  return 1;
}

