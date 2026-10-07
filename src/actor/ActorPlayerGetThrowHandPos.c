// bdc 0x088e19f8 ActorPlayerGetThrowHandPos
#include "bdc.h"

/* Writes the world position of the throwing hand bone into `out` (vec4): `Bip01_R_Hand` when
   `rightHand` is set, else `Bip01_L_Hand` (`GfxModelGetNodeWorldPos`). */

void ActorPlayerGetThrowHandPos(ActorPlayer *self, float *out)
{
  ScePspFVector4 posR __attribute__((aligned(16)));
  ScePspFVector4 posL __attribute__((aligned(16)));

  if (self->rightHand != '\0') {
    GfxModelGetNodeWorldPos((GfxModel *)self, &posR, "Bip01_R_Hand");
    out[0] = posR.x;
    out[1] = posR.y;
    out[2] = posR.z;
    out[3] = posR.w;
  } else {
    GfxModelGetNodeWorldPos((GfxModel *)self, &posL, "Bip01_L_Hand");
    out[0] = posL.x;
    out[1] = posL.y;
    out[2] = posL.z;
    out[3] = posL.w;
  }
}
