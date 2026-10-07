// bdc 0x089df874 GfxModelUpdateAndApplyMotion
#include "bdc.h"

/* Advances the model's motion (`GfxModelUpdateMotion`) and applies the pose to its nodes
   (`GfxModelApplyMotion`). */

void GfxModelUpdateAndApplyMotion(GfxModel *self)

{
  GfxModelUpdateMotion(self);
  GfxModelApplyMotion(self);
  return;
}

