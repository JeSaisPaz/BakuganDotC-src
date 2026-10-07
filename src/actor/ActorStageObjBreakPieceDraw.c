// bdc 0x088afbc4 ActorStageObjBreakPieceDraw
#include "bdc.h"

/* Draw method of the debris piece (vtable `0x08af29a4` slot 8): unconditionally writes the model
   display list (`GfxModelDlWriteState`). */

void ActorStageObjBreakPieceDraw(ActorStageObjBreakPiece *self, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)self,dl);
  return;
}

