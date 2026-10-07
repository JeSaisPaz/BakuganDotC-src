// bdc 0x089df380 GfxModelPlayMotionByName
#include "bdc.h"

/* Selects the motion `name` at frame `frame` (`GfxModelSelectMotionByName`) and, on success, sets its
   loop flag (`GfxModelSetMotionLoop`). Returns whether the motion was found. */

bool GfxModelPlayMotionByName(float frame, GfxModel *self, const char *name, bool loop)
{
  bool found = GfxModelSelectMotionByName(frame, self, name);
  if (found) {
    GfxModelSetMotionLoop(self, (u32)loop);
  }
  return found;
}
