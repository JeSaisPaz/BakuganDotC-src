// bdc 0x088a90a0 ActorStageObjMaterialSetTranslucent
#include "bdc.h"

/* Per-material callback (`GfxModelForEachMaterial`) of the stage objects: sets the blend mode
   bits 6..7 of the material flag byte `+4` to 1 (alpha blend), making the model translucent; used
   by the distance fade (`ActorStageObjUpdateFade`) when an object becomes see-through and by
   `ActorStageObjState00Update` / `ActorStageObjState05Update`. */

void ActorStageObjMaterialSetTranslucent(u8 *material)

{
  material[4] = material[4] & 0x3f | 0x40;
  return;
}

