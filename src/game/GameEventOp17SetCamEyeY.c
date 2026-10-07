// bdc 0x088ec4c4 GameEventOp17SetCamEyeY
#include "bdc.h"

/* Handler of event opcode 0x17 (`GameEventExecCommand`): sets `camKeys->eyeEnd[1]` to `arg` converted to
   20.12 fixed point (`((s64)arg << 24) / g_gameEventFixedScale` with `__divdi3`). `flag` is not used. */

void GameEventOp17SetCamEyeY(GameEvent *self, u8 flag, s16 arg)

{
  self->camKeys->eyeEnd[1] = (s32)__divdi3((s64)arg << 24, g_gameEventFixedScale);
  return;
}
