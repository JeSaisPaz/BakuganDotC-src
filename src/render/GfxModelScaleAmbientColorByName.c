// bdc 0x089e0664 GfxModelScaleAmbientColorByName
#include "bdc.h"

/* Like `GfxModelScaleAmbientColor` but only the materials whose name contains `name` (`strstr`)
   are touched: for each of their layer records (`GmoAttr`, 0x40 bytes) the packed colour
   `emission` (`+0x38`) is unpacked (each byte to about b/255, vuc2i + vi2f by 2^31), its alpha
   replaced by `scale`, every lane clamped to 0..1, scaled by 255 and repacked (vf2iz 23 + vi2uc). */

void GfxModelScaleAmbientColorByName(float scale, GfxModel *self, const char *name)
{
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < self->data->materialCount; i++) {
        if (strstr(GfxModelGetMaterialName(self, i), name) == NULL) {
            continue;
        }
        for (j = 0; j < ((GmoMaterial *)self->data->materials)[i].attrCount; j++) {
            GmoAttr *attr = &((GmoMaterial *)self->data->materials)[i].attrs[j];
            u32 packed = attr->emission;
            u32 result = 0;

            for (k = 0; k < 4; k++) {
                float f;

                if (k == 3) {
                    f = scale;
                } else {
                    u32 b = (packed >> (k * 8)) & 0xff;
                    f = (float)(int)(b * 0x01010101u >> 1) / 2147483648.0f;
                }
                result |= (u32)VfI2uc(VfF2iz(VfSat0(f) * 255.0f, 23)) << (k * 8);
            }
            attr->emission = result;
        }
    }
}
