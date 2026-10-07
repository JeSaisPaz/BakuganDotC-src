// bdc 0x089df4ec GfxModelGetMotionEnd
#include "bdc.h"

/* Returns the end frame (`range+4`) of the current motion slot (0x30-byte record `player+0x14 +
   model+0x134 * 0x30`, player = `model+0x130`). */

float GfxModelGetMotionEnd(GfxModel *self) {
    return GfxModelGetMotionRange(self)[1];
}
