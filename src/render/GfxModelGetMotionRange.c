// bdc 0x089dff3c GfxModelGetMotionRange
#include "bdc.h"

/* Returns a pointer to the `{start, end}` frame range of the current motion slot (0x30-byte record
   `player+0x14 + model+0x134 * 0x30`, player = `model+0x130`) (`GmoMotionGetData`). */

float *GfxModelGetMotionRange(GfxModel *self)
{
    return GmoMotionGetData((GmoMotionRecord *)self->data->motions + self->motionSlot);
}
