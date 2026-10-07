// bdc 0x088edb4c GameEventOp3BAddPropRotZ
#include "bdc.h"

/* Handler of event opcode 0x3b (`GameEventExecCommand`): sets the target angle z to the current
   angle (`+0x50`) plus `arg` degrees. */

void GameEventOp3BAddPropRotZ(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endRot[2] =
       self->props[flag].rot[2] + (short)(int)((float)(int)arg * 65536.0f * 0.0027777778f);
  return;
}

