// bdc 0x088ed980 GameEventOp36SetPropRotY
#include "bdc.h"

/* Handler of event opcode 0x36 (`GameEventExecCommand`): sets the target angle y (`record+0x54`)
   of prop slot `flag` to `arg` degrees. */

void GameEventOp36SetPropRotY(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endRot[1] = (s16)(int)((float)(int)arg * 65536.0f * 0.0027777778f);
  return;
}

