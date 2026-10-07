// bdc 0x089dfd80 GfxModelApplyMotion
#include "bdc.h"

/* When motion is enabled (`+0x13d`), applies the evaluated motion to the model's nodes
   (`GmoModelComputeWorldMatrices(player, 0xffff)`). */

void GfxModelApplyMotion(GfxModel *self)

{
  if (self->motionEnabled != '\0') {
    GmoModelComputeWorldMatrices(self->data);
  }
  return;
}

