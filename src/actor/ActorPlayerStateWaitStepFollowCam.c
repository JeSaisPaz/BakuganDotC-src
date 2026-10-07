// bdc 0x088e30f4 ActorPlayerStateWaitStepFollowCam
#include "bdc.h"

/* State 6 of the player actor (edit-man, `ActorPlayerCtor`) (vtable slot 26): with a camera
   (`+0x314`), starts its step-follow swing behind the player (mode 3,
   `GameFieldCameraBeginStepFollow`), waits until the camera's step counter `+0x304` reaches 2
   (swing finished, `GameFieldCameraFollowStep`), then resets the camera
   (`GameFieldCameraReset`) and returns to idle. Without a camera it returns to idle at once. */

void ActorPlayerStateWaitStepFollowCam(ActorPlayer *self)

{
  GameFieldCamera *cam;
  int state;
  
  cam = (self->base).camera;
  if (cam == (GameFieldCamera *)0x0) {
    ActorSetState(&self->base,0,'\0');
    return;
  }
  state = (self->base).waitTimer;
  if (state < 1) {
    if (-1 < state) {
      GameFieldCameraBeginStepFollow(cam);
      (self->base).waitTimer = (self->base).waitTimer + 1;
      return;
    }
  }
  else if (state < 2) {
    if (cam->stepState != 2) {
      return;
    }
    (self->base).waitTimer = state + 1;
    return;
  }
  GameFieldCameraReset(cam,'\0','\0');
  ActorSetState(&self->base,0,'\0');
  return;
}

