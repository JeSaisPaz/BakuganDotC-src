// bdc 0x088e29c8 ActorPlayerStateIdle
#include "bdc.h"

/* Idle state of the player actor (edit-man, `ActorPlayerCtor`) (vtable slot 20): returns at once
   (no velocity damping) if the camera `+0x314` is in mode 6 with `+0x5c4` null; otherwise reads the
   command flags `+0x168`: bit 0 walk (state 1), bit `0x400000` throw (state 7), the gauntlet command
   (`ActorPlayerWantsGauntletView`) state 11; then damps the X and Z velocity by 0.7. */

void ActorPlayerStateIdle(ActorPlayer *self)
{
  GameFieldCamera *cam = self->base.camera;
  float *vel;

  if (cam != NULL && self->base.camera->mode == 6 && self->base.camera->questCam == NULL) {
    return;
  }
  if ((self->base.motion & 1) != 0) {
    ActorSetState(&self->base, 1, 0);
  } else if ((self->base.motion & 0x400000) != 0) {
    ActorSetState(&self->base, 7, 0);
  } else if (ActorPlayerWantsGauntletView(self)) {
    ActorSetState(&self->base, 0xb, 0);
  }
  vel = self->base.base.velocity;
  vel[0] = vel[0] * 0.7f;
  vel[2] = vel[2] * 0.7f;
}
