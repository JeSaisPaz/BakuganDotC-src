// bdc 0x088b779c ActorBallModelMaterialCallback
#include "bdc.h"

/* Per-material callback (`GfxModelForEachMaterial`) of the ball-form Bakugan (`ActorBallCtor`):
   sets bits 5..7 of the material flag byte `+3` to 1 and bits 0..1 of byte `+4` to 2 (the `R`
   render code). */

void ActorBallModelMaterialCallback(u8 *material)

{
  material[3] = material[3] & 0x1f | 0x20;
  material[4] = material[4] & 0xfc | 2;
  return;
}

