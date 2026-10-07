// bdc 0x08a32418 CollisionSphereGetRadius
#include "bdc.h"

/* Get-radius method of the sphere shape (vtable `0x08af55c4` entry 11, offset `+0x5c`): returns the
   radius `+0x20`. */

float CollisionSphereGetRadius(void *sphere)

{
  return ((CollisionSphereQuery *)sphere)->radius;
}

