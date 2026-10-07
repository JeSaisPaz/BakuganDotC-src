// bdc 0x088bad5c GameFieldCameraBeginHold
#include "bdc.h"

/* Switches the field camera (`GameFieldCameraCtor`) to mode 5, in which the camera keeps its
   position (`GameFieldCameraModeHold`), and restores the near plane 3.9 (`+0x40`). */

void GameFieldCameraBeginHold(GameFieldCamera *cam)

{
  GameFieldCameraSetMode(cam,5);
  (cam->base).nearZ = 3.9f;
  return;
}

