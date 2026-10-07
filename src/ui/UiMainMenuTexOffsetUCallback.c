// bdc 0x089a38bc UiMainMenuTexOffsetUCallback
#include "bdc.h"

/* Material animation callback registered by `UiMainMenuHookSeaMaterial`
   (`GfxModelSetMaterialAnimCallback`): appends the GE texture-offset-U command (`0x4a`, float24
   of `value[0]`) to the display list `*dl` and advances it by one word; this scrolls the material's
   UVs. */
void UiMainMenuTexOffsetUCallback(u32 **dl, const float *value)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *value;
    **dl = (v.bits >> 8) | 0x4a000000;
    *dl = *dl + 1;
}
