// bdc 0x088ec3e8 GameEventOp13CamEnd
#include "bdc.h"

/* Handler of event opcode 0x13 (`GameEventExecCommand`): returns the camera `g_gfxActiveCamera`
   to normal control (`GameFieldCameraBeginTalkView(camera, 0)`). */

void GameEventOp13CamEnd(GameEvent *self, u8 flag, s16 arg)

{
  GameFieldCameraBeginTalkView((GameFieldCamera *)g_gfxActiveCamera);
  return;
}

