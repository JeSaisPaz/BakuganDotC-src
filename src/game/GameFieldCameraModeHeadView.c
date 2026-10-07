// bdc 0x088bd548 GameFieldCameraModeHeadView
#include "bdc.h"

/* Mode 8 update of the field camera (`GameFieldCameraCtor`): steps the head view helper `+0x3d0`
   (`GameFieldCameraHeadViewUpdate`) and fades the player when close
   (`GameFieldCameraFadeNearPlayer`). */

void GameFieldCameraModeHeadView(GameFieldCamera *cam)

{
  float *outEye;
  
  outEye = (cam->base).eye;
  if (cam->target != (void *)0x0) {
    GameFieldCameraHeadViewUpdate(cam->headView,outEye,(cam->base).target);
    GameFieldCameraFadeNearPlayer(outEye);
  }
  return;
}

