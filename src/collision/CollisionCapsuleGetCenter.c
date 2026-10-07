// bdc 0x08a32420 CollisionCapsuleGetCenter
#include "bdc.h"

/* Get-centre method of the capsule shape (vtable `0x08af5624` entry 8, offset `+0x44`): returns
   the start point of its axis (`start`, `capsule + 0x20`). */

ScePspFVector4 *CollisionCapsuleGetCenter(CollisionCapsule *capsule)

{
  return (ScePspFVector4 *)capsule->start;
}
