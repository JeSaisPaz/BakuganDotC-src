// bdc 0x088b05ac ActorStageObjScreenMaterialAdditive
#include "bdc.h"

/* Material callback of the screen object (`ActorStageObjScreenCtor`,
   `ActorStageObjScreenUpdate`): clears the two low blend bits of material byte 3 and sets bit 4
   of byte 4 (additive/unlit screen material). */

void ActorStageObjScreenMaterialAdditive(u8 *material)

{
  material[3] = material[3] & 0xfc;
  material[4] = material[4] | 0x10;
  return;
}

