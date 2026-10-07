// bdc 0x089e0314 GfxModelSetSpecular
#include "bdc.h"

/* Sets the specular colour and power of every layer record (`GmoAttr`, 0x40 bytes: `specular`
   `+0x30`, power float in `word3c` `+0x3c`) of every material of a GMO model object, skipping
   materials whose name (`GfxModelGetMaterialName`) contains `excludeName` (none skipped when
   NULL). The vec4 `colour` is clamped to 0..1, scaled by 255, truncated and packed to RGBA8888
   once up front. */

void GfxModelSetSpecular(float power, GfxModel *self, const float *colour, const char *excludeName)
{
    u32 packed;
    s32 i;
    s32 j;

    /* vsat0.q, vscl.q by S701 (255), vf2iz.q 23, vi2uc.q */
    packed = (u32)VfI2uc(VfF2iz(VfSat0(colour[0]) * 255.0f, 23)) |
             (u32)VfI2uc(VfF2iz(VfSat0(colour[1]) * 255.0f, 23)) << 8 |
             (u32)VfI2uc(VfF2iz(VfSat0(colour[2]) * 255.0f, 23)) << 16 |
             (u32)VfI2uc(VfF2iz(VfSat0(colour[3]) * 255.0f, 23)) << 24;

    for (i = 0; i < self->data->materialCount; i++) {
        if (excludeName != NULL &&
            strstr(GfxModelGetMaterialName(self, i), excludeName) != NULL) {
            continue;
        }
        for (j = 0; j < ((GmoMaterial *)self->data->materials)[i].attrCount; j++) {
            GmoAttr *attr = &((GmoMaterial *)self->data->materials)[i].attrs[j];

            attr->specular = packed;
            *(float *)&attr->word3c = power;
        }
    }
}
