// bdc 0x089dfd40 GfxModelAdvanceMotion
#include "bdc.h"

/* Advances the model's motion player (`+0x130`) by `frameTime (0x08ac5c7c) / 30 * speed (+0xb4)`
   with all channels (`GmoMotionUpdate`). */

void GfxModelAdvanceMotion(GfxModel *self) {
    /* 0.0333333f is the ELF's 0x3d088880, a hair under 1/30 (0.033333335f is 0x3d088889): motion frames step
       0.999999 a frame at 30 fps, so a looping motion shows its end frame before wrapping and a cross-fade
       keeps a tiny weight one frame longer (guards' frozen head yaw, story 06) */
    GmoMotionUpdate(g_gfxMotionTimeScale * 0.0333333f * self->motionSpeed, self->data, 0xffff);
}
