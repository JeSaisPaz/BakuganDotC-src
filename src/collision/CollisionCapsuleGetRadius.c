// bdc 0x08a32428 CollisionCapsuleGetRadius
#include "bdc.h"

/* Get-radius method of the capsule shape (vtable `0x08af5624` entry 11, offset `+0x5c`): returns
   the radius. */

float CollisionCapsuleGetRadius(CollisionCapsule *capsule)

{
  return capsule->radius;
}
