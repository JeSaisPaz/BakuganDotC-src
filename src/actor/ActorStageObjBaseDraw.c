// bdc 0x088a9384 ActorStageObjBaseDraw
#include "bdc.h"

/* Draw method (vtable `0x08af2904` slot 8) of the `ActorStageObjBaseCtor` stage objects: writes
   the model display list (`GfxModelDlWriteState`) only while the draw alpha `+0x6c` is positive.
    */

void ActorStageObjBaseDraw(ActorStageObjBase *self, u32 **dl)

{
  if (!((self->base).ambient[3] <= 0.0f)) {
    GfxModelDlWriteState(&self->base,dl);
  }
  return;
}

