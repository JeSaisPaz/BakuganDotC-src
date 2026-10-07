// bdc 0x088e8908 ActorNpcRobotModelMaterialCallback
#include "bdc.h"

/* Per-material callback (`GfxModelForEachMaterial`) installed by `ActorNpcRobotSetupModel`:
   sets bits 5..7 of the material flag byte `+3` to 2 and bits 0..1 of byte `+4` to 2 (the `R`
   render code). */

void ActorNpcRobotModelMaterialCallback(u8 *material)

{
  material[3] = material[3] & 0x1f | 0x40;
  material[4] = material[4] & 0xfc | 2;
  return;
}

