// bdc 0x088edadc GameEventOp3AAddPropRotY
#include "bdc.h"

/* Handler of event opcode 0x3a (`GameEventExecCommand`): sets the target angle y to the current
   angle (`+0x4e`) plus `arg` degrees. */

void GameEventOp3AAddPropRotY(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endRot[1] =
       self->props[flag].rot[1] + (short)(int)((float)(int)arg * 65536.0f * 0.0027777778f);
  return;
}

