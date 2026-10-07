// bdc 0x089df50c GfxModelSetMotionProgress
#include "bdc.h"

/* Moves the current motion to `t` of its length (frame = `(int)(end * t)`) and returns the previous
   progress (`frame / end`, 1 when the end is 0). */

float GfxModelSetMotionProgress(GfxModel *self, float t)

{
  float frame;
  float end;

  frame = GfxModelGetMotionFrame(self);
  end = GfxModelGetMotionEnd(self);
  GfxModelSetMotionFrame(self, (float)(int)(end * t));
  end = GfxModelGetMotionEnd(self);
  if (end != 0.0f) {
    return frame / end;
  }
  return 1.0f;
}
