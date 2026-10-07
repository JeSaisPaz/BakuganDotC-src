// bdc 0x088e0a9c ActorStateRestorePlacement
#include "bdc.h"

/* State 8 handler of the base actor (vtable `0x08af37e4` slot 28): re-applies the placement record
   (`ActorApplyPlacement`). */
void ActorStateRestorePlacement(Actor *self)
{
    ActorApplyPlacement(self);
}
