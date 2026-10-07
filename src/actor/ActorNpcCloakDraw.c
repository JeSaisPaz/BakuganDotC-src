// bdc 0x088e5584 ActorNpcCloakDraw
#include "bdc.h"

/* Draw method of the cloaked guard class (model 0x4f, `ActorNpcCloakCtor`, vtable `0x08af39e4`)
   slot 8: draws only when the mode `+0x460` is non-zero, with GE `PMSKC` set to the mode's mask
   from `0x08a98ca8` around `GfxModelDlWriteState`. */

void ActorNpcCloakDraw(ActorNpcCloak *self, u32 **dl)

{
  if (self->mode != 0) {
    **dl = g_actorCloakPmskc[self->mode] & 0xffffff | 0xe8000000;
    *dl = *dl + 1;
    GfxModelDlWriteState((GfxModel *)self,dl);
    **dl = 0xe8000000;
    *dl = *dl + 1;
  }
  return;
}

