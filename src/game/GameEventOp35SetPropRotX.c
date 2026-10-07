// bdc 0x088ed91c GameEventOp35SetPropRotX
#include "bdc.h"

/* Handler of event opcode 0x35 (`GameEventExecCommand`): sets the target angle x (`record+0x52`,
   s16) of prop slot `flag` to `arg` degrees. */

void GameEventOp35SetPropRotX(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endRot[0] = (s16)(int)((float)(int)arg * 65536.0f * 0.0027777778f);
  return;
}

