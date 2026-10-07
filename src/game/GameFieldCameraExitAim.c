// bdc 0x088bd66c GameFieldCameraExitAim
#include "bdc.h"

/* Mode 4 exit handler of the field camera (`GameFieldCameraCtor`): restores the player's alpha
   through the aim helper (`GameFieldCameraAimViewExit`). */

void GameFieldCameraExitAim(GameFieldCamera *cam)

{
  GameFieldCameraAimViewExit(cam->aimView);
  return;
}

