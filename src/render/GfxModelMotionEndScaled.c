// bdc 0x089df4bc GfxModelMotionEndScaled
#include "bdc.h"

/* Returns `(int)(motionEnd * t)` for the model's current motion (`GfxModelGetMotionEnd`). */

s32 GfxModelMotionEndScaled(GfxModel *self, float t) {
    return (s32)(GfxModelGetMotionEnd(self) * t);
}
