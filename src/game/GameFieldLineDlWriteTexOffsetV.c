// bdc 0x088bded4 GameFieldLineDlWriteTexOffsetV
#include "bdc.h"

/* Material animation callback bound by `GameFieldPhaseLoad` to the `psp_line__BA` material
   (`GfxModelSetMaterialAnimCallback`, value = model `+0x80`): appends GE command 0x4B (texture V
   offset) with `*value` to `*dl`, scrolling the line texture. */
void GameFieldLineDlWriteTexOffsetV(u32 **dl, const float *value)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *value;
    **dl = (v.bits >> 8) | 0x4b000000;
    *dl = *dl + 1;
}
