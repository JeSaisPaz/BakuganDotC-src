// bdc 0x088ed208 GameEventOp26SetCamFov
#include "bdc.h"

/* Handler of event opcode 0x26 (`GameEventExecCommand`): sets the field-of-view end key
   (`key+0x62`) to `arg`. */

void GameEventOp26SetCamFov(GameEvent *self, u8 flag, s16 arg)

{
  self->camKeys->fovEnd = flag;
  return;
}

