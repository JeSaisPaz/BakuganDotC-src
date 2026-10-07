// bdc 0x088ece80 GameEventOp1CMoveCamEyeZ
#include "bdc.h"

/* Handler of event opcode 0x1c (`GameEventExecCommand`): adds `arg` along axis 2 (in the current
   coordinate mode, `GameEventApplyAxisOffset`) to the eye end key. */

void GameEventOp1CMoveCamEyeZ(GameEvent *self, s16 arg)
{
  s32 off[3];

  off[0] = 0;
  off[1] = 0;
  off[2] = 0;
  GameEventApplyAxisOffset(self, 2, arg, off, 0);
  self->camKeys->eyeEnd[0] += off[0];
  self->camKeys->eyeEnd[1] += off[1];
  self->camKeys->eyeEnd[2] += off[2];
}
