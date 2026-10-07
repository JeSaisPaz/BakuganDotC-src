// bdc 0x088ed9e4 GameEventOp37SetPropRotZ
#include "bdc.h"

/* Handler of event opcode 0x37 (`GameEventExecCommand`): sets the target angle z (`record+0x56`)
   of prop slot `flag` to `arg` degrees. */

void GameEventOp37SetPropRotZ(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endRot[2] = (s16)(int)((float)(int)arg * 65536.0f * 0.0027777778f);
  return;
}

