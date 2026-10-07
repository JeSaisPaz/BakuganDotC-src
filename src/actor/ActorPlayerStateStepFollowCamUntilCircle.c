// bdc 0x088e3024 ActorPlayerStateStepFollowCamUntilCircle
#include "bdc.h"

/* State 5 of the player actor (edit-man, `ActorPlayerCtor`) (vtable slot 25): with a camera
   (`+0x314`), starts its step-follow swing behind the player (mode 3,
   `GameFieldCameraBeginStepFollow`) and makes the analog stick act as the d-pad
   (`g_padState``->stickEmulatesDpad`); waits for command bit `0x20000` (set by
   `BtlInputReadActions` when Circle is released), then resets the camera behind the player
   (`GameFieldCameraReset`), clears the stick emulation and returns to idle. Without a camera it
   returns to idle at once. */

void ActorPlayerStateStepFollowCamUntilCircle(ActorPlayer *self)

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
      g_padState->stickEmulatesDpad = '\x01';
      (self->base).waitTimer = (self->base).waitTimer + 1;
      return;
    }
  }
  else if (state < 2) {
    if (((self->base).motion & 0x20000U) == 0) {
      return;
    }
    GameFieldCameraReset(cam,'\0','\0');
    (self->base).waitTimer = (self->base).waitTimer + 1;
    return;
  }
  ActorSetState(&self->base,0,'\0');
  g_padState->stickEmulatesDpad = '\0';
  return;
}

