// bdc 0x088a6714 ActorStageObjWindGeneratorDraw
#include "bdc.h"

/* Draw method (vtable `0x08af2714` slot 8) of the `ActorStageObjWindGeneratorCtor` stage objects:
   writes the model display list (`GfxModelDlWriteState`) only while the draw alpha `+0x6c` is
   positive. */

void ActorStageObjWindGeneratorDraw(ActorStageObjWindGenerator *self, u32 **dl)

{
  if (!((self->base).base.ambient[3] <= 0.0f)) {
    GfxModelDlWriteState((GfxModel *)self,dl);
  }
  return;
}

