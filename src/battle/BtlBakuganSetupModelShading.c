// bdc 0x0885e220 BtlBakuganSetupModelShading
#include "bdc.h"

/* Sets up the per-kind shading of a Bakugan model (a `GfxModel`; nothing for kinds `base.unk08`
   0x21 and up): enables `lighting`, stores `g_colorBlack` packed to RGBA8 (each lane clamped to
   [0, 1], scaled by 255, truncated, lane x in the low byte) in `fogColor`, runs `BtlBakuganMaterialSetAlphaRef` on every material
   (`GfxModelForEachMaterial`), and adds a specular highlight (colour 0.4/0.4/0.4/1.0, power 8,
   `GfxModelSetSpecular`, no excluded material) when the high nibble of byte 3 of the kind's stat
   record (`g_btlKindStatTables`), read as a signed 4-bit value, is non-zero, and again when the
   kind is 0xe. `model` stays untyped because the UI callers pass their own model handles. */
void BtlBakuganSetupModelShading(void *model)
{
    GfxModel *self = (GfxModel *)model;
    /* GfxModelSetSpecular reads the colour with lv.q: 16-byte aligned (sp+0 / sp+0x10). */
    float specular[4] __attribute__((aligned(16)));
    float specular2[4] __attribute__((aligned(16)));
    u32 packed;
    s8 nibble;

    if (self->base.unk08 >= 0x21) {
        return;
    }
    self->lighting = 1;
    packed = (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.x) * 255.0f, 23)) |
             (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.y) * 255.0f, 23)) << 8 |
             (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.z) * 255.0f, 23)) << 16 |
             (u32)VfI2uc(VfF2iz(VfSat0(g_colorBlack.w) * 255.0f, 23)) << 24;
    self->fogColor = packed;
    GfxModelForEachMaterial(self, (void *)BtlBakuganMaterialSetAlphaRef, NULL);
    nibble = (s8)((((g_btlKindStatTables[self->base.unk08][3] & 0xf0) >> 4) ^ 8) - 8);
    if (nibble != 0) {
        specular[0] = 0.400000006f;
        specular[1] = 0.400000006f;
        specular[2] = 0.400000006f;
        specular[3] = 1.0f;
        GfxModelSetSpecular(8.0f, self, specular, NULL);
    }
    if (self->base.unk08 == 0xe) {
        specular2[0] = 0.400000006f;
        specular2[1] = 0.400000006f;
        specular2[2] = 0.400000006f;
        specular2[3] = 1.0f;
        GfxModelSetSpecular(8.0f, self, specular2, NULL);
    }
}
