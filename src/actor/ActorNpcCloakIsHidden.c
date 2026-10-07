// bdc 0x088e5568 ActorNpcCloakIsHidden
#include "bdc.h"

/* True when the cloaked guard's visibility mode `+0x460` is 0 (hidden); the field HUD radar skips
   hidden guards. */

bool ActorNpcCloakIsHidden(ActorNpcCloak *self)

{
  if (self->mode == 0) {
    return true;
  }
  return false;
}

