// bdc 0x088bad0c GameFieldCameraBeginAimView
#include "bdc.h"

/* Switches the field camera (`GameFieldCameraCtor`) to mode 4 (throw aiming,
   `GameFieldCameraModeAim`) and resets the aim helper `+0x400` from the current eye/look-at
   (`GameFieldCameraAimViewBegin`). */
void GameFieldCameraBeginAimView(GameFieldCamera *cam)
{
  float view[8];
  int i;

  GameFieldCameraSetMode(cam, 4);
  /* view = { eye, look-at } */
  for (i = 0; i < 4; i++)
    view[i] = cam->base.eye[i];
  for (i = 0; i < 4; i++)
    view[4 + i] = cam->base.target[i];
  GameFieldCameraAimViewBegin((GameFieldCameraAimView *)cam->aimView, view);
}
