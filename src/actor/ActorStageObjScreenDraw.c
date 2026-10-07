// bdc 0x088b0900 ActorStageObjScreenDraw
#include "bdc.h"

/* Draw method (vtable `0x08af2a44` slot 8) of the `ActorStageObjScreenCtor` stage objects: writes
   the model display list (`GfxModelDlWriteState`) only while the draw alpha `+0x6c` is positive.
    */

void ActorStageObjScreenDraw(ActorStageObjScreen *self, u32 **dl)

{
  if (!((self->base).base.ambient[3] <= 0.0f)) {
    GfxModelDlWriteState((GfxModel *)self,dl);
  }
  return;
}

