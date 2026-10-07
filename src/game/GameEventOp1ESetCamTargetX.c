// bdc 0x088ecf1c GameEventOp1ESetCamTargetX
#include "bdc.h"

/* Handler of event opcode 0x1e (`GameEventExecCommand`): sets `camKeys->targetEnd[0]` to `arg` converted to
   20.12 fixed point (`((s64)arg << 24) / g_gameEventFixedScale` with `__divdi3`). `flag` is not used. */

void GameEventOp1ESetCamTargetX(GameEvent *self, u8 flag, s16 arg)

{
  self->camKeys->targetEnd[0] = (s32)__divdi3((s64)arg << 24, g_gameEventFixedScale);
  return;
}
