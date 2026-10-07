// bdc 0x088bd39c GameFieldCameraModeTerminal
#include "bdc.h"

/* Mode 2 update of the field camera (`GameFieldCameraCtor`)
   (`GameFieldCameraBeginTerminalView`): keeps the view while the target's state `+0x140` is 4
   (using a terminal) and otherwise resets behind it (`GameFieldCameraReset`). */

void GameFieldCameraModeTerminal(GameFieldCamera *cam)

{
  if (((Actor *)cam->target)->state != 4) {
    GameFieldCameraReset(cam,'\0','\0');
  }
  return;
}
