// bdc 0x088e158c ActorPlayerDraw
#include "bdc.h"

/* Draw method of the player actor (edit-man, `ActorPlayerCtor`), vtable slot 8: while stealthed
   (`+0x3a0`) it wraps `GfxModelDlWriteState` with GE `PMSKC 0xffffff` / `PMSKC 0` (all colour
   writes masked), so only depth is written and the player is invisible. */

void ActorPlayerDraw(ActorPlayer *self, u32 **dl)

{
  if (self->stealth != '\0') {
    **dl = 0xe8ffffff;
    *dl = *dl + 1;
  }
  GfxModelDlWriteState((GfxModel *)self,dl);
  if (self->stealth != '\0') {
    **dl = 0xe8000000;
    *dl = *dl + 1;
  }
  return;
}

