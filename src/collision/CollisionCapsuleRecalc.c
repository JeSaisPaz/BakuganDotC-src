// bdc 0x08a297b0 CollisionCapsuleRecalc
#include "bdc.h"

/* Recomputes the derived data of a capsule shape (vtable `0x08af5624` entry 9, offset `+0x4c`):
   `radiusSq` from `radius`, then `axisLen` as the length of `axis` (VFPU `vdot.t` + `vsqrt.s`
   on scratch registers, no VFPU value crosses the return). */

void CollisionCapsuleRecalc(CollisionCapsule *capsule)

{
  capsule->radiusSq = capsule->radius * capsule->radius;
  capsule->axisLen = __builtin_sqrtf(capsule->axis[0] * capsule->axis[0] +
                                     capsule->axis[1] * capsule->axis[1] +
                                     capsule->axis[2] * capsule->axis[2]);
}
