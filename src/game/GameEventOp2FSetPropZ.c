// bdc 0x088ed528 GameEventOp2FSetPropZ
#include "bdc.h"

/* Handler of event opcode 0x2f (`GameEventExecCommand`): sets the end-position z (`record+0x14`)
   of prop slot `flag` from `arg` (20.12 fixed point): `arg` is shifted by 12 in 32 bits, then
   by another 12 as a 64-bit value, and divided by `g_gameEventFixedScale` with `__divdi3`. */

void GameEventOp2FSetPropZ(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endPos[2] = (s32)__divdi3((s64)arg << 24, g_gameEventFixedScale);
}
