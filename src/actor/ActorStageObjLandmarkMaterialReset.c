// bdc 0x088a186c ActorStageObjLandmarkMaterialReset
#include "bdc.h"

/* Per-material callback of `ActorStageObjLandmarkSetupMaterials` (`GfxModelForEachMaterial`):
   clears bit 4 of the flag byte `+4` and zeroes byte `+6` of every material state record of the
   landmark model. */

void ActorStageObjLandmarkMaterialReset(u8 *material)

{
  material[6] = '\0';
  material[4] = material[4] & 0xef;
  return;
}

