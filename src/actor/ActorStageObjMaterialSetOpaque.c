// bdc 0x088a90b8 ActorStageObjMaterialSetOpaque
#include "bdc.h"

/* Per-material callback (`GfxModelForEachMaterial`) of the stage-object distance fade
   (`ActorStageObjUpdateFade`): clears the blend mode bits 6..7 of the material flag byte `+4`,
   making the model opaque again. */

void ActorStageObjMaterialSetOpaque(u8 *material)

{
  material[4] = material[4] & 0x3f;
  return;
}

