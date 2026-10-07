// bdc 0x089e0430 GfxModelSetAmbientColor
#include "bdc.h"

/* Sets the ambient material colour (with alpha) of every attribute record (`GmoAttr`, 0x40 bytes,
   field `emission` at `+0x38`) of every material of a GMO model object, skipping materials whose
   name (`GfxModelGetMaterialName`) contains `excludeName` (all when NULL). The vec4 `colour` is
   clamped to 0..1, scaled by 255, truncated and packed to RGBA8888 once up front. */

void GfxModelSetAmbientColor(GfxModel *self, const float *colour, const char *excludeName)
{
    u32 packed;
    s32 index;
    s32 j;
    GmoMaterial *mat;

    /* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q */
    packed = (u32)VfI2uc(VfF2iz(VfSat0(colour[0]) * 255.0f, 23)) |
             (u32)VfI2uc(VfF2iz(VfSat0(colour[1]) * 255.0f, 23)) << 8 |
             (u32)VfI2uc(VfF2iz(VfSat0(colour[2]) * 255.0f, 23)) << 16 |
             (u32)VfI2uc(VfF2iz(VfSat0(colour[3]) * 255.0f, 23)) << 24;

    for (index = 0; index < self->data->materialCount; index++) {
        if (excludeName != NULL &&
            strstr(GfxModelGetMaterialName(self, index), excludeName) != NULL) {
            continue;
        }
        for (j = 0; j < ((GmoMaterial *)self->data->materials)[index].attrCount; j++) {
            mat = &((GmoMaterial *)self->data->materials)[index];
            mat->attrs[j].emission = packed; /* ambient colour, see GmoDlWriteMaterialColors */
        }
    }
}
