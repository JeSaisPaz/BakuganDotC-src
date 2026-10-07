// bdc 0x08a159f0 GmoModelSelectMotion
#include "bdc.h"

/* Selects motion `index` of a GMO model data block: stores `frame` in `motionBlend` and the index in
   `motionIndex`, resolves the 0x30-byte motion record (NULL when the low 16 bits of `index` exceed
   `motionCount`; an `index` whose `index + 1` overflows 16 bits is passed through raw) and clears
   its user word to 0.0f (`GmoMotionSetUser`). A NULL model does nothing. */

void GmoModelSelectMotion(float frame, GmoModel *self, u32 index)

{
  GmoMotionRecord *motion;

  if (self != NULL) {
    self->motionBlend = frame;
    self->motionIndex = (s16)index;
    motion = (GmoMotionRecord *)(uintptr_t)index;
    if (((index + 1) & 0xffff0000) == 0) {
      motion = NULL;
      if ((index & 0xffff) <= (u32)self->motionCount) {
        motion = (GmoMotionRecord *)self->motions + index;
      }
    }
    GmoMotionSetUser(motion, 0.0f);
  }
}
