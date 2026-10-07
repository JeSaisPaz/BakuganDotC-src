// bdc 0x088e13fc ActorPlayerIsIdleOrThrowing
#include "bdc.h"

/* True when the player is in state 0 (idle) or 7 (throw). */

bool ActorPlayerIsIdleOrThrowing(ActorPlayer *self)

{
  if ((self->base).state != 0 && (self->base).state != 7) {
    return false;
  }
  return true;
}
