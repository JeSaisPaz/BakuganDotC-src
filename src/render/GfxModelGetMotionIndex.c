// bdc 0x089dffe4 GfxModelGetMotionIndex
#include "bdc.h"

/* Returns the model's registry motion index `+0x138` (-1 when none). */
s32 GfxModelGetMotionIndex(GfxModel *self)
{
    return self->motionIndex;
}
