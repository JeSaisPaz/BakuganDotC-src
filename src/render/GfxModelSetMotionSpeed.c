// bdc 0x089df758 GfxModelSetMotionSpeed
#include "bdc.h"

/* Sets the model's motion speed `+0xb4` (frames per 1/30 s, negative = backwards) and returns the
   previous value. */

float GfxModelSetMotionSpeed(GfxModel *self, float speed)
{
    float old = self->motionSpeed;
    self->motionSpeed = speed;
    return old;
}
