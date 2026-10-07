// bdc 0x088a4804 ActorStageObjEggCrystalDraw
#include "bdc.h"

/* Draw method (vtable `0x08af2524` / `0x08af25c4` slot 8) of the `ActorStageObjEggCrystalCtor`
   stage objects: writes the model display list (`GfxModelDlWriteState`) only while the draw alpha
   `+0x6c` is positive. */

void ActorStageObjEggCrystalDraw(ActorStageObjEggCrystal *self, u32 **dl)

{
  if (!((self->base).base.ambient[3] <= 0.0f)) {
    GfxModelDlWriteState((GfxModel *)self,dl);
  }
  return;
}

