// bdc 0x088e1a7c ActorGetLeftUpperArmPos
#include "bdc.h"

/* Writes the world position of the `Bip01_L_UpperArm` bone of `actor` into `out` (`GfxModelGetNodeWorldPos`).
    */

void ActorGetLeftUpperArmPos(void *actor, float *out)

{
  GfxModelGetNodeWorldPos(actor,(ScePspFVector4 *)out,"Bip01_L_UpperArm");
  return;
}

