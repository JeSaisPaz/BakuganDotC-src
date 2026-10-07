// bdc 0x089df618 GfxModelGetMotionRemaining
#include "bdc.h"

/* Returns `end - frame` for the current motion. */

float GfxModelGetMotionRemaining(GfxModel *self) {
    float frame = GfxModelGetMotionFrame(self);
    return GfxModelGetMotionEnd(self) - frame;
}
