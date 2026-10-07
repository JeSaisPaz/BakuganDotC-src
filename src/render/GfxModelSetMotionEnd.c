// bdc 0x089df700 GfxModelSetMotionEnd
#include "bdc.h"

/* Sets the end frame of the current motion and returns the previous one. */

float GfxModelSetMotionEnd(GfxModel *self, float end) {
    float *range = GfxModelGetMotionRange(self);
    float old = range[1];
    range[1] = end;
    return old;
}
