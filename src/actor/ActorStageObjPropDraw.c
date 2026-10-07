// bdc 0x088b0cb0 ActorStageObjPropDraw
#include "bdc.h"

/* Draw method (vtable `0x08af2ae4` slot 8) of the `ActorStageObjPropCtor` stage objects: writes
   the model display list (`GfxModelDlWriteState`) only while the draw alpha `+0x6c` is positive.
    */

void ActorStageObjPropDraw(ActorStageObjProp *self, u32 **dl)

{
  if (!((self->base).base.ambient[3] <= 0.0f)) {
    GfxModelDlWriteState((GfxModel *)self,dl);
  }
  return;
}

