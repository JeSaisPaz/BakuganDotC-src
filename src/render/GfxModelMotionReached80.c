// bdc 0x089df6a0 GfxModelMotionReached80
#include "bdc.h"

/* Returns whether the current motion is at least 80 % done (`GfxModelMotionReached` with 0.8). */

bool GfxModelMotionReached80(GfxModel *self)

{
  if (GfxModelMotionReached(self,0.8f)) {
    return true;
  }
  return false;
}
