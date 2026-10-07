// bdc 0x089df31c GfxModelSelectMotionByName
#include "bdc.h"

/* Selects the motion called `name` on a model at frame `frame` (`GfxModelMotionIndexOfName` then
   `GfxModelSelectMotion`); returns false when the motion does not exist. */

bool GfxModelSelectMotionByName(float frame, GfxModel *self, const char *name)
{
  s32 index;

  index = GfxModelMotionIndexOfName(self, name);
  if (index != -1) {
    return GfxModelSelectMotion(frame, self, index);
  }
  return false;
}
