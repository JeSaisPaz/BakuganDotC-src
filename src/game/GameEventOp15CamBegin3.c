// bdc 0x088ec434 GameEventOp15CamBegin3
#include "bdc.h"

/* Handler of event opcode 0x15 (`GameEventExecCommand`): same body as `GameEventOp12CamBegin`.
    */

void GameEventOp15CamBegin3(GameEvent *self, u8 flag, s16 arg)

{
  GameEventCaptureCamera(self);
  GameFieldCameraBeginHold((GameFieldCamera *)g_gfxActiveCamera);
  return;
}

