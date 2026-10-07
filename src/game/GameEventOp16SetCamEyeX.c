// bdc 0x088ec45c GameEventOp16SetCamEyeX
#include "bdc.h"

/* Handler of event opcode 0x16 (`GameEventExecCommand`): sets the eye end key x (`*(ev+0x2c) +
   0xc`) to `arg` converted to 20.12 fixed point (`((s64)arg << 24) / g_gameEventFixedScale` with
   the 64-bit `__divdi3`). */

void GameEventOp16SetCamEyeX(GameEvent *self, s16 arg)

{
  self->camKeys->eyeEnd[0] = (s32)__divdi3((s64)arg << 24, g_gameEventFixedScale);
  return;
}
