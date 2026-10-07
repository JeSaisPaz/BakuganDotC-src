// bdc 0x088b3f58 ActorStageObjCrystalMaterialSetTranslucent
#include "bdc.h"

/* Per-material callback (`GfxModelForEachMaterial`) of the crystal stage object: sets the blend
   mode bits 6..7 of the material flag byte `+4` to 1 (alpha blend), making the model translucent,
   used when it breaks (`ActorStageObjCrystalBreak`). */

void ActorStageObjCrystalMaterialSetTranslucent(u8 *material)

{
  material[4] = material[4] & 0x3f | 0x40;
  return;
}

