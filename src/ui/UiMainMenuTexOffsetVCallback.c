// bdc 0x089a3890 UiMainMenuTexOffsetVCallback
#include "bdc.h"

/* Material animation callback registered by `UiMainMenuHookLineMaterial`
   (`GfxModelSetMaterialAnimCallback`): appends the GE texture-offset-V command (`0x4b`, float24
   of `value[0]`) to the display list `*dl` and advances it by one word; this scrolls the material's
   UVs. */
void UiMainMenuTexOffsetVCallback(u32 **dl, const float *value)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *value;
    **dl = (v.bits >> 8) | 0x4b000000;
    *dl = *dl + 1;
}
