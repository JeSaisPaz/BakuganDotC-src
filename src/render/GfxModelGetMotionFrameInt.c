// bdc 0x089df488 GfxModelGetMotionFrameInt
#include "bdc.h"

/* Returns the current motion frame as an integer, `floor(frame + 0.1)`. */

s32 GfxModelGetMotionFrameInt(GfxModel *self)
{
    return (s32)__builtin_floorf(GfxModelGetMotionFrame(self) + 0.1f);
}
