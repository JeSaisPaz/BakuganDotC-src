// bdc 0x08a32478 CollisionBoxVsBoxFalse
#include "bdc.h"

/* Box shape method for box queries (vtable `0x08af5684` entry 5): unsupported pair, always returns
   false. */

bool CollisionBoxVsBoxFalse(CollisionBox *self, void *other, ScePspFVector4 *out)

{
  return false;
}

