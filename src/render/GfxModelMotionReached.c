// bdc 0x089df654 GfxModelMotionReached
#include "bdc.h"

/* Returns 1 when the current motion's progress (`GfxModelGetMotionProgress`) is at least `t`. */

bool GfxModelMotionReached(GfxModel *self, float t)

{
  float progress;

  progress = GfxModelGetMotionProgress(self);
  if (progress < t) {
    return false;
  }
  return true;
}

