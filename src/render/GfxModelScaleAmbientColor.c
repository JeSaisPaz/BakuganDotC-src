// bdc 0x089e053c GfxModelScaleAmbientColor
#include "bdc.h"

/* Sets the ambient alpha of the materials of a GMO model object to `scale`: for each layer record
   (`GmoAttr`, 0x40 bytes) the packed colour `emission` (`+0x38`) is unpacked (each byte to about
   b/255, vuc2i + vi2f by 2^31), its alpha replaced by `scale`, every lane clamped to 0..1, scaled
   by 255 and repacked (vf2iz 23 + vi2uc), so RGB go through unchanged. Touches every material
   when `nameFilter` is NULL, otherwise every material except those whose name
   (`GfxModelGetMaterialName`) contains `nameFilter`. */

void GfxModelScaleAmbientColor(float scale, GfxModel *self, const char *nameFilter)
{
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < self->data->materialCount; i++) {
        if (nameFilter != NULL &&
            strstr(GfxModelGetMaterialName(self, i), nameFilter) != NULL) {
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
