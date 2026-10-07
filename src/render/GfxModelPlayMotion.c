// bdc 0x089df3d0 GfxModelPlayMotion
#include "bdc.h"

/* Selects motion `index` with blend time `blend` (passed through in `f12`,
   `GfxModelSelectMotion`) and, on success, sets its loop flag from the low byte of `loop`
   (`GfxModelSetMotionLoop`). */

bool GfxModelPlayMotion(float blend, GfxModel *self, s32 index, u32 loop)
{
  bool found = GfxModelSelectMotion(blend, self, index);
  if (found) {
    GfxModelSetMotionLoop(self, loop & 0xff);
  }
  return found;
}
