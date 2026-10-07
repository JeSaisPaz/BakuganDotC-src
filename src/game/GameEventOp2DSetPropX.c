// bdc 0x088ed418 GameEventOp2DSetPropX
#include "bdc.h"

/* Handler of event opcode 0x2d (`GameEventExecCommand`): sets the end-position x (`record+0xc`)
   of event prop slot `flag` (100-byte records at `ev+0x30`, prop object at `+0x30`, see
   `GameEventPropCreate`) from `arg` (20.12 fixed point). */

void GameEventOp2DSetPropX(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endPos[0] = (s32)__divdi3((s64)arg << 24, g_gameEventFixedScale);
}
