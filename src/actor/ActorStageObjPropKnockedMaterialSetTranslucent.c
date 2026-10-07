// bdc 0x088b09a8 ActorStageObjPropKnockedMaterialSetTranslucent
#include "bdc.h"

/* Per-material callback (`GfxModelForEachMaterial`) of the knock-over prop: sets the blend mode
   bits 6..7 of the material flag byte `+4` to 1 (alpha blend), making the model translucent, used
   twice by `ActorStageObjPropState02Knocked`. */

void ActorStageObjPropKnockedMaterialSetTranslucent(u8 *material)

{
  material[4] = material[4] & 0x3f | 0x40;
  return;
}

