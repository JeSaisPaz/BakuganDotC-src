// bdc 0x089df46c GfxModelMotionFrame
#include "bdc.h"

/* Returns the current frame of the model's motion (`GfxModelGetMotionFrame`); a separate compiled
   wrapper. */

float GfxModelMotionFrame(GfxModel *self)
{
    return GfxModelGetMotionFrame(self);
}
