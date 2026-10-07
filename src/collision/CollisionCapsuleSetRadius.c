// bdc 0x08a2a1c0 CollisionCapsuleSetRadius
#include "bdc.h"

/* Sets the radius of a capsule shape (vtable `0x08af5624` entry 10, offset `+0x54`): stores
   `radius` and its square in `radiusSq`. */

void CollisionCapsuleSetRadius(CollisionCapsule *capsule, float radius)

{
  capsule->radius = radius;
  capsule->radiusSq = radius * radius;
}
