// bdc 0x08a323e0 CollisionSphereGetCenter
#include "bdc.h"

/* Get-centre method of the sphere shape (vtable `0x08af55c4` entry 8, offset `+0x44`): returns
   `sphere + 0x10`, its centre. */

ScePspFVector4 *CollisionSphereGetCenter(void *sphere)

{
  return &((CollisionSphereQuery *)sphere)->center;
}

