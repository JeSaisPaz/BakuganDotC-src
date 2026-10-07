// bdc 0x089df764 GfxModelGetMotionSpeed
#include "bdc.h"

/* Returns the model's motion speed. */
float GfxModelGetMotionSpeed(GfxModel *self)
{
    return self->motionSpeed;
}
