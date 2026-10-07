// bdc 0x088a2148 ActorStageObjLandmarkDraw
#include "bdc.h"

/* Draw method (vtable `0x08af2434` slot 8) of the `ActorStageObjLandmarkCtor` stage objects:
   writes the model display list (`GfxModelDlWriteState`) only while the draw alpha `+0x6c` is
   positive. */

void ActorStageObjLandmarkDraw(ActorStageObjLandmark *self, u32 **dl)

{
  if (!((self->base).base.ambient[3] <= 0.0f)) {
    GfxModelDlWriteState((GfxModel *)self,dl);
  }
  return;
}

