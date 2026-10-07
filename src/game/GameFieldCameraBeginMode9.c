// bdc 0x088bae08 GameFieldCameraBeginMode9
#include "bdc.h"

/* Switches the field camera (`GameFieldCameraCtor`) to mode 9 and starts the helper `+0x3c0` (`GameFieldCameraMode9Begin`, a spring view built from the player->`obj` direction); needs a target. */

void GameFieldCameraBeginMode9(GameFieldCamera *cam, void *obj)

{
  float v[12];
  int i;

  if (cam->target != (void *)0x0) {
    GameFieldCameraSetMode(cam,9);
    for (i = 0; i < 4; i++) v[0+i] = ((const float *)obj)[i];
    for (i = 0; i < 4; i++) v[4+i] = cam->base.eye[i];
    for (i = 0; i < 4; i++) v[8+i] = cam->base.target[i];
    GameFieldCameraMode9Begin((void **)cam->mode9,v);
  }
}
