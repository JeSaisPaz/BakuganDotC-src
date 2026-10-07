// bdc 0x088bd600 GameFieldCameraExitHeadView
#include "bdc.h"

/* Mode 8 exit handler of the field camera (`GameFieldCameraCtor`): calls the head view helper's
   empty exit (`GameFieldCameraHeadViewExit`). */

void GameFieldCameraExitHeadView(GameFieldCamera *cam)

{
  GameFieldCameraHeadViewExit(cam->headView);
  return;
}

