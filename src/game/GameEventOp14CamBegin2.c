// bdc 0x088ec40c GameEventOp14CamBegin2
#include "bdc.h"

/* Handler of event opcode 0x14 (`GameEventExecCommand`): same body as `GameEventOp12CamBegin`.
    */

void GameEventOp14CamBegin2(GameEvent *self, u8 flag, s16 arg)

{
  GameEventCaptureCamera(self);
  GameFieldCameraBeginHold((GameFieldCamera *)g_gfxActiveCamera);
  return;
}

