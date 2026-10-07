// bdc 0x088b8760 ActorBallDraw
#include "bdc.h"

/* Draw (vtable slot 8) of the `ActorBall` (`ActorBallCtor`): forwards to
   `GfxModelDlWriteState`. */

void ActorBallDraw(CoreObject *ball, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)ball,dl);
  return;
}

