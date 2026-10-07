// bdc 0x08858cdc ActorCrystalDlWrite
#include "bdc.h"

/* Crystal vtable slot 8 (display-list write): forwards to `GfxModelDlWriteState`. */

void ActorCrystalDlWrite(ActorCrystal *self, u32 **dl)

{
  GfxModelDlWriteState((GfxModel *)self,dl);
  return;
}

