// bdc 0x088ec52c GameEventOp18SetCamEyeZ
#include "bdc.h"

/* Handler of event opcode 0x18 (`GameEventExecCommand`): sets `camKeys->eyeEnd[2]` to `arg` converted to
   20.12 fixed point (`((s64)arg << 24) / g_gameEventFixedScale` with `__divdi3`). `flag` is not used. */

void GameEventOp18SetCamEyeZ(GameEvent *self, s16 arg, u8 flag)

{
  self->camKeys->eyeEnd[2] = (s32)__divdi3((s64)arg << 24, g_gameEventFixedScale);
  return;
}
