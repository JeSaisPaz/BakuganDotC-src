// bdc 0x088e3a34 ActorPlayerStateReact
#include "bdc.h"

/* State 13 of the player actor (edit-man, `ActorPlayerCtor`) (table `0x08a98b4c`, entered when a
   guard bumps into the stealthed player): drops stealth (`ActorPlayerStealthOff`), plays motion
   slot 0x1b at speed 16 and returns to idle when it ends. */

void ActorPlayerStateReact(ActorPlayer *self)

{
  int state;
  float remaining;
  
  state = (self->base).waitTimer;
  if (state < 1) {
    if (-1 < state) {
      ActorPlayerStealthOff(self,'\0');
      ActorPlayMotion(0.2,self,0x1b,'\0','\0');
      GfxModelSetMotionEnd((GfxModel *)self,16.0);
      (self->base).waitTimer = 1;
    }
  }
  else if (state < 2) {
    remaining = GfxModelGetMotionRemaining((GfxModel *)self);
    if (remaining == 0.0) {
      (self->base).waitTimer = 2;
    }
  }
  else if (state < 3) {
    ActorSetState(&self->base,0,'\0');
  }
  return;
}

