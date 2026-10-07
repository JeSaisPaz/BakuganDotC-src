// bdc 0x088a5fdc ActorStageObjMineDraw
#include "bdc.h"

/* Draw method (vtable `0x08af2674` slot 8) of the `ActorStageObjMineCtor` stage objects: writes
   the model display list (`GfxModelDlWriteState`) only while the draw alpha `+0x6c` is positive.
    */

void ActorStageObjMineDraw(ActorStageObjMine *self, u32 **dl)

{
  if (!((self->base).base.ambient[3] <= 0.0f)) {
    GfxModelDlWriteState((GfxModel *)self,dl);
  }
  return;
}

