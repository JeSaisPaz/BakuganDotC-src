// bdc 0x0882a654 BtlSwordBlurDraw
#include "bdc.h"

/* Draws the sword-blur trail (`BtlSwordBlurInit`) unless its alpha (`color[3]`) is 0 or below (called
   by `BtlBakuganDraw`): writes GE commands at `*cursor` – additive blend, material colour/alpha
   from `color` (saturated to 0..1, scaled by 255 and truncated to bytes), UV scale (1.0, 0.3) and offset (0, 0.03), the 4x3 part of `g_gfxIdentityMatrix` as the
   world matrix (float24 operands ORed with `g_btlSwordBlurWorldCmd`), the `"swordblur"` texture
   (`GfxTextureWriteCall`, slot 1 when the owner's kind is 0x12) – then an inline 4×8 spline control net
   jumped over by the list: for each of the 8 ring slots, newest first, the tip point, two points
   interpolated towards the base point (factors 0.3333 and 0.6666),
   and the base point (xyz only). It then sets the vertex type, the vertex address, draws the
   spline (`SPLINE` 0x060f0804: 4×8 control points, open ends), restores the UV scale/offset and advances `*cursor` past the commands.
   Nothing is written when the alpha is 0 or below (a NaN alpha still draws). */
static inline u32 BtlSwordBlurBits(float value)
{
    u32 bits;

    memcpy(&bits, &value, sizeof(bits));
    return bits;
}


static inline u32 BtlSwordBlurFloat24(float value)
{
    u32 bits;

    memcpy(&bits, &value, sizeof(bits));
    return bits >> 8;
}

void BtlSwordBlurDraw(BtlSwordBlur *self, u32 **cursor)
{
    float point[3];
    const ScePspFVector4 *rows[4];
    u32 *list = *cursor;
    u32 *patch;
    u32 *end;
    u32 cmd;
    u32 rgba;
    s32 slot;
    s32 i;
    s32 m;

    if (self->color[3] <= 0.0f) {
        return;
    }
    list[0] = 0xdf000032;
    list[1] = 0xe0000000;
    list[2] = 0xe1000000;

    /* colour -> packed RGBA bytes: saturated to 0..1, scaled by 255 (VFPU bank constant S701) */
    {
        u32 lane[4];

        for (i = 0; i < 4; i++) {
            lane[i] = VfI2uc(VfF2iz(VfSat0(self->color[i]) * 255.0f, 23));
        }
        rgba = lane[0] | lane[1] << 8 | lane[2] << 16 | lane[3] << 24;
    }
    list[3] = (rgba & 0xffffff) | 0x55000000;
    list[4] = (rgba >> 24) | 0x58000000;
    list[5] = 0x1e000001;
    list[6] = 0x17000000;
    list[7] = 0xe7000001;
    list[8] = 0x53000000;
    list[9] = 0x1d000000;
    list[10] = 0x36001804;
    list[11] = 0x22000000;
    list[12] = 0x24000000;
    list[13] = BtlSwordBlurFloat24(1.0f) | 0x48000000;
    list[14] = BtlSwordBlurFloat24(0.300000012f) | 0x49000000;
    list[15] = BtlSwordBlurFloat24(0.0f) | 0x4a000000;
    list[16] = BtlSwordBlurFloat24(0.0299999993f) | 0x4b000000;
    list[17] = 0x3a000000;
    cmd = g_btlSwordBlurWorldCmd & 0xff000000;
    rows[0] = &g_gfxIdentityMatrix.x;
    rows[1] = &g_gfxIdentityMatrix.y;
    rows[2] = &g_gfxIdentityMatrix.z;
    rows[3] = &g_gfxIdentityMatrix.w;
    for (m = 0; m < 4; m++) {
        list[18 + m * 3] = cmd | BtlSwordBlurFloat24(rows[m]->x);
        list[19 + m * 3] = cmd | BtlSwordBlurFloat24(rows[m]->y);
        list[20 + m * 3] = cmd | BtlSwordBlurFloat24(rows[m]->z);
    }
    list = GfxTextureWriteCall(self->texture, list + 30, self->owner->base.base.unk08 == 0x12);

    /* jump over the inline control points */
    patch = list + 2;
    end = patch + 0x60;
    {
        uintptr_t addr = (uintptr_t)end;

        list[0] = (u32)(((addr >> 24) & 0xf) << 16) | 0x10000000;
        list[1] = (u32)(addr & 0xffffff) | 0x08000000;
    }

    slot = self->head;
    list = patch;
    for (i = 0; i < 8; i++) {
        list[0] = BtlSwordBlurBits(self->tip[slot][0]);
        list[1] = BtlSwordBlurBits(self->tip[slot][1]);
        list[2] = BtlSwordBlurBits(self->tip[slot][2]);
        point[0] = self->tip[slot][0] + (self->base[slot][0] - self->tip[slot][0]) * 0.3333f;
        point[1] = self->tip[slot][1] + (self->base[slot][1] - self->tip[slot][1]) * 0.3333f;
        point[2] = self->tip[slot][2] + (self->base[slot][2] - self->tip[slot][2]) * 0.3333f;
        list[3] = BtlSwordBlurBits(point[0]);
        list[4] = BtlSwordBlurBits(point[1]);
        list[5] = BtlSwordBlurBits(point[2]);
        point[0] = self->tip[slot][0] + (self->base[slot][0] - self->tip[slot][0]) * 0.6666f;
        point[1] = self->tip[slot][1] + (self->base[slot][1] - self->tip[slot][1]) * 0.6666f;
        point[2] = self->tip[slot][2] + (self->base[slot][2] - self->tip[slot][2]) * 0.6666f;
        list[6] = BtlSwordBlurBits(point[0]);
        list[7] = BtlSwordBlurBits(point[1]);
        list[8] = BtlSwordBlurBits(point[2]);
        list[9] = BtlSwordBlurBits(self->base[slot][0]);
        list[10] = BtlSwordBlurBits(self->base[slot][1]);
        list[11] = BtlSwordBlurBits(self->base[slot][2]);
        slot = (slot - 1) & 7;
        list += 12;
    }

    end[0] = 0x12000180;
    list = end + 1;
    if (patch != NULL) {
        uintptr_t addr = (uintptr_t)patch;

        list[0] = (u32)(((addr >> 24) & 0xf) << 16) | 0x10000000;
        list[1] = (u32)(addr & 0xffffff) | 0x01000000;
        list += 2;
    }
    list[0] = 0x060f0804;
    list[1] = 0xe7000000;
    list[2] = 0x36000202;
    list[3] = 0x53000003;
    list[4] = BtlSwordBlurFloat24(1.0f) | 0x48000000;
    list[5] = BtlSwordBlurFloat24(1.0f) | 0x49000000;
    list[6] = BtlSwordBlurFloat24(0.0f) | 0x4a000000;
    list[7] = BtlSwordBlurFloat24(0.0f) | 0x4b000000;
    *cursor = list + 8;
}
