// bdc 0x088b05cc ActorStageObjScreenMaterialAdditiveHidden
#include "bdc.h"

/* Material callback of the screen object applied while it is hidden (`ActorStageObjScreenUpdate`,
   battle camera mode 6): clears bits 0..1 of the material flag byte `+3` and sets bit 4 of byte
   `+4`, exactly like `ActorStageObjScreenMaterialAdditive`. */

void ActorStageObjScreenMaterialAdditiveHidden(u8 *material)

{
  material[3] = material[3] & 0xfc;
  material[4] = material[4] | 0x10;
  return;
}

