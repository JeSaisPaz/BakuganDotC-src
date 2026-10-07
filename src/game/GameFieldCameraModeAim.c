// bdc 0x088bd3cc GameFieldCameraModeAim
#include "bdc.h"

/* Mode 4 update of the field camera (`GameFieldCameraCtor`): steps the aim helper `+0x400` into
   the eye/look-at (`GameFieldCameraAimViewUpdate`) and fades the player when close
   (`GameFieldCameraFadeNearPlayer`). */

void GameFieldCameraModeAim(GameFieldCamera *cam)

{
  float *outEye;
  
  outEye = (cam->base).eye;
  if (cam->target != (void *)0x0) {
    GameFieldCameraAimViewUpdate(cam->aimView,outEye,(cam->base).target);
    GameFieldCameraFadeNearPlayer(outEye);
  }
  return;
}

