// bdc 0x089dffd4 GfxModelIsMotion
#include "bdc.h"

/* Returns whether the model's registry motion index `+0x138` (set by `GfxModelSelectMotion`)
   equals `index`. */

bool GfxModelIsMotion(GfxModel *self, s32 index)

{
  return self->motionIndex == index;
}

