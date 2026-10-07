// bdc 0x088e1144 ActorPlayerCancelPowers
#include "bdc.h"

/* Forces both powers off (`ActorPlayerStealthOff`, `ActorPlayerScanOff`) and clears the
   cooldown `+0x3a8`. */

void ActorPlayerCancelPowers(ActorPlayer *self)

{
  ActorPlayerStealthOff(self,'\x01');
  ActorPlayerScanOff(self,'\x01');
  self->powerCooldown = 0;
  return;
}

