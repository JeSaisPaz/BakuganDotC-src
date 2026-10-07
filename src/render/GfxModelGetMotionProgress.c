// bdc 0x089df5ac GfxModelGetMotionProgress
#include "bdc.h"

/* Returns the progress of the current motion, `frame / end` (1.0 when the end frame is 0). */

float GfxModelGetMotionProgress(GfxModel *self)

{
  float frame;
  float end;
  
  frame = GfxModelGetMotionFrame(self);
  end = GfxModelGetMotionEnd(self);
  if (end != 0.0f) {
    return frame / end;
  }
  return 1.0f;
}

