// bdc 0x088bcf20 GameFieldCameraSnapQuestCam
#include "bdc.h"

/* Sets the snap flag `+0x4a` of the field camera's quest camera controller `+0x5c4`
   (`GameFieldCameraCtor`), if any, so that its next step jumps to the target without easing:
   `GameQuestCamCtrlCtor` sets the same flag for its first step, `GameQuestCamCtrlStep` clears
   it after each step, and `GameQuestCamSpringAccelerate` zeroes the spring motion while it is
   set. */

void GameFieldCameraSnapQuestCam(GameFieldCamera *cam)
{
  if (cam->questCam != (void *)0x0) {
    ((GameQuestCamCtrl *)cam->questCam)->snap = 1;
  }
}
