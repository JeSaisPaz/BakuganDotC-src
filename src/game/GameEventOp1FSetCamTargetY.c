// bdc 0x088ecf84 GameEventOp1FSetCamTargetY
#include "bdc.h"

/* Handler of event opcode 0x1f (`GameEventExecCommand`): sets `camKeys->targetEnd[1]` to `arg` converted to
   20.12 fixed point (`((s64)arg << 24) / g_gameEventFixedScale` with `__divdi3`). `flag` is not used. */

void GameEventOp1FSetCamTargetY(GameEvent *self, s16 arg, u8 flag)

{
  self->camKeys->targetEnd[1] = (s32)__divdi3((s64)arg << 24, g_gameEventFixedScale);
  return;
}
