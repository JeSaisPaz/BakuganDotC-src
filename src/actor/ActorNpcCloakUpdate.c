// bdc 0x088e5804 ActorNpcCloakUpdate
#include "bdc.h"

/* Per-frame update of the cloaked guard class (model 0x4f, `ActorNpcCloakCtor`, vtable
   `0x08af39e4`) (slot 34): unless in state 4, reveals the guard (mode 2) while the player's scan
   power is on (`player+0x3a1`); otherwise drops back from mode 2 and every 90 frames plays sound
   `0x2c0003b` and flickers (mode 3, `ActorNpcCloakUpdateFlicker`). Then runs the guard update
   `ActorNpcGuardUpdate`. */

void ActorNpcCloakUpdate(ActorNpcCloak *self)

{
  ActorPlayer *player;
  int timer;
  
  player = (ActorPlayer *)ActorFindPlayer();
  ((BtlShadow *)self->base.base.base.shadow)->enabled = 0;
  if ((self->base).base.aiState != 4) {
    if (player->scan == 0) {
      if (self->mode == 2) {
        ActorNpcCloakSetMode(self,0);
      }
      timer = self->flickerTimer + 1;
      self->flickerTimer = timer;
      if (timer % 0x5a == 0) {
        SndObjectAddEmitter((self->base).base.base.base.sound,0x2c0003b,'\0','\0');
        ActorNpcCloakSetMode(self,3);
        self->flickerFrame = 0;
        self->flickerTimer = 0;
        self->flickerPhase = 0;
      }
      ActorNpcCloakUpdateFlicker(self);
    }
    else if (self->mode != 2) {
      ActorNpcCloakSetMode(self,2);
      self->flickerTimer = 0;
    }
  }
  ActorNpcGuardUpdate(&self->base);
  return;
}

