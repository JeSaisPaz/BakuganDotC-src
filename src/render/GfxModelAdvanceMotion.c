// bdc 0x089dfd40 GfxModelAdvanceMotion
#include "bdc.h"

/* Advances the model's motion player (`+0x130`) by `frameTime (0x08ac5c7c) / 30 * speed (+0xb4)`
   with all channels (`GmoMotionUpdate`). */

void GfxModelAdvanceMotion(GfxModel *self) {
    GmoMotionUpdate(g_gfxMotionTimeScale * 0.033333335f * self->motionSpeed, self->data, 0xffff);
}
