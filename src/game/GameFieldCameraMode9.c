// bdc 0x088bd588 GameFieldCameraMode9
#include "bdc.h"

/* Mode 9 update of the field camera (`GameFieldCameraCtor`): steps the helper `+0x3c0`
   (`GameFieldCameraMode9Update`) into eye/look-at and fades the player when close
   (`GameFieldCameraFadeNearPlayer`). */

void GameFieldCameraMode9(GameFieldCamera *cam)

{
  float *outEye;
  
  outEye = (cam->base).eye;
  if (cam->target != (void *)0x0) {
    GameFieldCameraMode9Update((void **)cam->mode9,outEye,(cam->base).target);
    GameFieldCameraFadeNearPlayer(outEye);
  }
  return;
}

