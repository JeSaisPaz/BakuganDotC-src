// bdc 0x088a8e98 ActorStageObjDraw
#include "bdc.h"

/* Draw method (vtable `0x08af2864` slot 8) of the `ActorStageObjCtor` stage objects: writes the
   model display list (`GfxModelDlWriteState`) only while the draw alpha `+0x6c` is positive. */

void ActorStageObjDraw(ActorStageObj *self, u32 **dl)

{
  if (!((self->base).base.ambient[3] <= 0.0f)) {
    GfxModelDlWriteState((GfxModel *)self,dl);
  }
  return;
}

