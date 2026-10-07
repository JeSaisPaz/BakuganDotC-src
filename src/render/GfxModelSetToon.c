// bdc 0x089e092c GfxModelSetToon
#include "bdc.h"

/* Sets the toon-shading level (bits 2..4 of `GfxMaterialState.shadeFlags` = `level + 1`) on every
   material whose `shadeFlags` bits 5..7 are clear, except materials whose name contains
   `"notoon"` (their bits 2..4 are cleared); with `enable` false clears bits 2..4 on all of them. */

void GfxModelSetToon(GfxModel *self, bool enable, s32 level)
{
    GfxMaterialState *state;
    s32 count;
    s32 index;
    u8 flags;

    count = self->materialCount;
    for (index = 0; index < count; index++) {
        state = (GfxMaterialState *)GfxModelGetMaterialState(self, index);
        if ((state->shadeFlags & 0xe0) != 0) {
            continue;
        }
        if (enable) {
            const char *hit = strstr(GfxModelGetMaterialName(self, index), "notoon");
            flags = state->shadeFlags & 0xe3;
            if (hit == NULL) {
                flags |= (((u8)(level + 1)) & 7) << 2;
            }
            state->shadeFlags = flags;
        } else {
            state->shadeFlags &= 0xe3;
        }
    }
}
