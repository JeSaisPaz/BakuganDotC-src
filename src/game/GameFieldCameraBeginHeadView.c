// bdc 0x088bad94 GameFieldCameraBeginHeadView
#include "bdc.h"

/* Switches the field camera (`GameFieldCameraCtor`) to mode 8 and starts the head view helper `+0x3d0` (`GameFieldCameraHeadViewBegin`, looking from the player's `Bip01_Head` towards `obj`); needs a target. */

void GameFieldCameraBeginHeadView(GameFieldCamera *cam, void *obj)

{
  float v[12];
  int i;

  if (cam->target != (void *)0x0) {
    GameFieldCameraSetMode(cam,8);
    for (i = 0; i < 4; i++) v[8+i] = ((const float *)obj)[i];
    for (i = 0; i < 4; i++) v[0+i] = cam->base.eye[i];
    for (i = 0; i < 4; i++) v[4+i] = cam->base.target[i];
    GameFieldCameraHeadViewBegin(cam->headView,v);
  }
}
