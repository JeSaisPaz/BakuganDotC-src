// bdc 0x088ed4a0 GameEventOp2ESetPropY
#include "bdc.h"

/* Handler of event opcode 0x2e (`GameEventExecCommand`): sets the end-position y (`record+0x10`)
   of prop slot `flag`. */

void GameEventOp2ESetPropY(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endPos[1] = (s32)__divdi3((s64)arg << 24, g_gameEventFixedScale);
}
