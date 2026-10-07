// bdc 0x088ecd88 GameEventOp1AMoveCamEyeX
#include "bdc.h"

/* Handler of event opcode 0x1a (`GameEventExecCommand`): adds `arg` along axis 0 (in the current
   coordinate mode, `GameEventApplyAxisOffset`) to the eye end key. */

void GameEventOp1AMoveCamEyeX(GameEvent *self, s16 arg)
{
  s32 off[3];

  off[0] = 0;
  off[1] = 0;
  off[2] = 0;
  GameEventApplyAxisOffset(self, 0, arg, off, 0);
  self->camKeys->eyeEnd[0] += off[0];
  self->camKeys->eyeEnd[1] += off[1];
  self->camKeys->eyeEnd[2] += off[2];
}
