// bdc 0x089df6e0 GfxModelGetMotionStart
#include "bdc.h"

/* Returns the start frame (`range+0`) of the current motion slot (0x30-byte record `player+0x14 +
   model+0x134 * 0x30`, player = `model+0x130`). */

float GfxModelGetMotionStart(GfxModel *self) {
    return GfxModelGetMotionRange(self)[0];
}
