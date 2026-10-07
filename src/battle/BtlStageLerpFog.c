// bdc 0x0889cda8 BtlStageLerpFog
#include "bdc.h"

/* Interpolates two arena fog records (`BtlArenaFog`, see `BtlStageGetFogParams`) by `t` into
   `out`: `range` and `scale` linearly (`a + (b - a) * t`), the packed RGBA `color` per channel
   (bytes unpacked to floats in [0, 1], `a`'s left in `scratch[0..3]`, lerped there, saturated to
   [0, 1], scaled by 255 and repacked to bytes). Used by `BtlStageUpdateAmbientEffects`. */
void BtlStageLerpFog(float t, BtlArenaFog *out, float *scratch, const BtlArenaFog *a,
                     const BtlArenaFog *b)
{
    float bColor[4];
    u32 aPacked;
    u32 bPacked;
    u32 packed;
    int i;

    out->range = a->range + (b->range - a->range) * t;
    out->scale = a->scale + (b->scale - a->scale) * t;
    /* vuc2i + vi2f 31: byte * 0x01010101 >> 1, divided by 2^31 */
    aPacked = a->color;
    for (i = 0; i < 4; i++) {
        u32 byte = (aPacked >> (i * 8)) & 0xff;
        scratch[i] = (float)(int)(byte * 0x01010101u >> 1) / 2147483648.0f;
    }
    bPacked = b->color;
    for (i = 0; i < 4; i++) {
        u32 byte = (bPacked >> (i * 8)) & 0xff;
        bColor[i] = (float)(int)(byte * 0x01010101u >> 1) / 2147483648.0f;
    }
    for (i = 0; i < 4; i++) {
        float d = bColor[i] - scratch[i];
        d = d * t;
        scratch[i] = scratch[i] + d;
    }
    packed = 0;
    for (i = 0; i < 4; i++) {
        float c = VfSat0(scratch[i]) * 255.0f;
        packed |= (u32)VfI2uc(VfF2iz(c, 23)) << (i * 8);
    }
    out->color = packed;
}
