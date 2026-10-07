// bdc 0x088b4ab4 ActorStageObjCrystalDraw
#include "bdc.h"

/* Draw method (vtable `0x08af2b94` slot 8) of the `ActorStageObjCrystalCtor` stage objects:
   writes the model display list (`GfxModelDlWriteState`) only while the draw alpha `+0x6c` is
   positive. */

void ActorStageObjCrystalDraw(ActorStageObjCrystal *self, u32 **dl)

{
  if (!((self->base).base.ambient[3] <= 0.0f)) {
    GfxModelDlWriteState((GfxModel *)self,dl);
  }
  return;
}

