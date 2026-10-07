// bdc 0x088e1420 ActorPlayerIsWalking
#include "bdc.h"

/* True when the player is in state 1 (walk). */

bool ActorPlayerIsWalking(ActorPlayer *self)

{
  if ((self->base).state == 1) {
    return true;
  }
  return false;
}

