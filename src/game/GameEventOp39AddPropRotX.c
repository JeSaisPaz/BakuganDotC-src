// bdc 0x088eda6c GameEventOp39AddPropRotX
#include "bdc.h"

/* Handler of event opcode 0x39 (`GameEventExecCommand`): sets the target angle x of prop slot
   `flag` to its current angle (`+0x4c`) plus `arg` degrees. */

void GameEventOp39AddPropRotX(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endRot[0] =
       self->props[flag].rot[0] + (u16)(int)((float)arg * 65536.0f * 0.0027777778f);
  return;
}
