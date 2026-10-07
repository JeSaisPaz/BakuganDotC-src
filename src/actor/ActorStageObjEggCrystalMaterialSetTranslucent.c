// bdc 0x088a3ab8 ActorStageObjEggCrystalMaterialSetTranslucent
#include "bdc.h"

/* Per-material callback (`GfxModelForEachMaterial`) of the egg crystal: sets the blend mode bits
   6..7 of the material flag byte `+4` to 1 (alpha blend), making the model translucent while it
   appears or breaks (`ActorStageObjEggCrystalSummon`, `ActorStageObjEggCrystalBreak`,
   `ActorStageObjEggCrystalState01Appear`). */

void ActorStageObjEggCrystalMaterialSetTranslucent(u8 *material)

{
  material[4] = material[4] & 0x3f | 0x40;
  return;
}

