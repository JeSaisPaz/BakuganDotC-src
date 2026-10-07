// bdc 0x089df72c GfxModelSetMotionStart
#include "bdc.h"

/* Sets the start frame of the current motion and returns the previous one (used by
   `GameGimmickSwitchSetPose`). */

float GfxModelSetMotionStart(GfxModel *self, float start) {
    float *range = GfxModelGetMotionRange(self);
    float old = range[0];
    range[0] = start;
    return old;
}
