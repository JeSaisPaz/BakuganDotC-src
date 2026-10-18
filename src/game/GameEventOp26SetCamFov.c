// bdc 0x088ed208 GameEventOp26SetCamFov
#include "bdc.h"

/* Handler of event opcode 0x26 (`GameEventExecCommand`): sets the field-of-view end key
   (`key+0x62`) to `fov`, the
   16-bit command argument (`(u16)arg`). */

void GameEventOp26SetCamFov(GameEvent *self, u16 fov, s16 unused)

{
  self->camKeys->fovEnd = fov;
  return;
}

