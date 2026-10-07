// bdc 0x088ecfec GameEventOp20SetCamTargetZ
#include "bdc.h"

/* Handler of event opcode 0x20 (`GameEventExecCommand`): sets the target end key z (`key+0x44`)
   to `arg` converted to 20.12 fixed point (`((s64)arg << 24) / g_gameEventFixedScale`). The value
   is read from the second argument register (`a1`); the third is unused. */

void GameEventOp20SetCamTargetZ(GameEvent *self, s16 arg, s16 unused)

{
  self->camKeys->targetEnd[2] = (s32)__divdi3((s64)arg << 24, g_gameEventFixedScale);
  return;
}
