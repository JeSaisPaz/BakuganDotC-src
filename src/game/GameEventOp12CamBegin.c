// bdc 0x088ec3c0 GameEventOp12CamBegin
#include "bdc.h"

/* Handler of event opcode 0x12 (`GameEventExecCommand`): captures the current camera into the key
   record (`GameEventCaptureCamera`) and switches the camera `g_gfxActiveCamera` to event
   control (`GameFieldCameraBeginHold`). */

void GameEventOp12CamBegin(GameEvent *self, u8 flag, s16 arg)

{
  GameEventCaptureCamera(self);
  GameFieldCameraBeginHold((GameFieldCamera *)g_gfxActiveCamera);
  return;
}

