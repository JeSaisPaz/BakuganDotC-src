// bdc 0x088ec3e8 GameEventOp13CamEnd
#include "bdc.h"

/* Handler of event opcode 0x13 (`GameEventExecCommand`): switches the field camera
   `g_gfxActiveCamera` to the talk view framing its target and the target's talk partner
   (`GameFieldCameraBeginTalkView`). The binary also loads `a1 = 0` for the call, but the callee
   never reads it (its first instruction overwrites `a1`), so the C passes the camera only. */

void GameEventOp13CamEnd(GameEvent *self, u8 flag, s16 arg)

{
  GameFieldCameraBeginTalkView((GameFieldCamera *)g_gfxActiveCamera);
  return;
}

