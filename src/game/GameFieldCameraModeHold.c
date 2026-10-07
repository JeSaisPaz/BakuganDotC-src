// bdc 0x088bd40c GameFieldCameraModeHold
#include "bdc.h"

/* Mode 5 update of the field camera (`GameFieldCameraCtor`): leaves eye and look-at alone; only
   when coming from mode 8 (`+0x2a8`) keeps fading the player (`GameFieldCameraFadeNearPlayer`).
    */

void GameFieldCameraModeHold(GameFieldCamera *cam)

{
  if (cam->prevMode == 8) {
    GameFieldCameraFadeNearPlayer((cam->base).eye);
  }
  return;
}

