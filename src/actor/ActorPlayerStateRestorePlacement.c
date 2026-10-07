// bdc 0x088e31a0 ActorPlayerStateRestorePlacement
#include "bdc.h"

/* State 8 of the player actor (edit-man, `ActorPlayerCtor`) (vtable slot 28): cancels both powers
   and re-applies the placement record (`ActorApplyPlacement`). */

void ActorPlayerStateRestorePlacement(ActorPlayer *self)

{
  ActorPlayerCancelPowers(self);
  ActorApplyPlacement(&self->base);
  return;
}

