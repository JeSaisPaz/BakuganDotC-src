// bdc 0x08a2c6c8 GameEventApplyAxisOffsetNop
#include "bdc.h"

/* Entry 13 (`+0x6c`) of the field event task base vtable `0x08af41ec`: empty; task 470 overrides it
   with `GameEvent470ApplyAxisOffset`. */

void GameEventApplyAxisOffsetNop(GameEvent *self, s32 axis, s16 amount, s32 *vec, u8 actorIdx)

{
  return;
}

