// bdc 0x0886066c BtlBakuganGetFootPosition
#include "bdc.h"

/* Returns a pointer to the static vector g_btlFootPos, filled with the world position of the
   unit's foot `foot` (2 = left, anything else = right). Kind 0xc: model node 0x1d (left) or
   0x1f (right); its world matrix (model rootMatrix * node localMatrix) transforms the local
   point g_btlFootNodeOffset = (100, 0, 0, 1), set up on first use. Kind 10: the anchor position
   of leg effect legs[i], i = (variant == 0 ? 2 : 0) + (foot == 3 ? 1 : 0). Other kinds: the
   world position of the "Bip01_L_Foot" (foot 2) or "Bip01_R_Foot" bone. */

float *BtlBakuganGetFootPosition(BtlBakugan *self, int foot, int variant)
{
    float world[16];
    ScePspFVector4 bonePos;
    const float *a;
    const float *b;
    const float *t;
    GmoModel *data;
    GmoNode *node;
    GfxEffect *leg;
    int legIndex;
    int j;
    int r;

    if (self->base.base.unk08 == 0xc) {
        if (g_btlFootNodeOffsetInit == 0) {
            g_btlFootNodeOffsetInit = 1;
            g_btlFootNodeOffset[0] = 100.0f;
            g_btlFootNodeOffset[1] = 0.0f;
            g_btlFootNodeOffset[2] = 0.0f;
            g_btlFootNodeOffset[3] = 1.0f;
        }
        data = self->base.data;
        if (foot == 2) {
            node = GfxModelGetNode(&self->base, 0x1d);
        } else {
            node = GfxModelGetNode(&self->base, 0x1f);
        }
        /* vmmul.q M000, M100, M200: world = rootMatrix * localMatrix (column-major,
           column j = sum_k b[j][k] * column k of a) */
        a = data->rootMatrix;
        b = node->localMatrix;
        for (j = 0; j < 4; j++) {
            for (r = 0; r < 4; r++) {
                world[j * 4 + r] = b[j * 4 + 0] * a[0 * 4 + r] + b[j * 4 + 1] * a[1 * 4 + r] +
                                   b[j * 4 + 2] * a[2 * 4 + r] + b[j * 4 + 3] * a[3 * 4 + r];
            }
        }
        /* vtfm4.q C000, E100, C200: g_btlFootPos = sum_k offset[k] * column k of world */
        t = g_btlFootNodeOffset;
        g_btlFootPos[0] = world[0] * t[0] + world[4] * t[1] + world[8] * t[2] + world[12] * t[3];
        g_btlFootPos[1] = world[1] * t[0] + world[5] * t[1] + world[9] * t[2] + world[13] * t[3];
        g_btlFootPos[2] = world[2] * t[0] + world[6] * t[1] + world[10] * t[2] + world[14] * t[3];
        g_btlFootPos[3] = world[3] * t[0] + world[7] * t[1] + world[11] * t[2] + world[15] * t[3];
    } else if (self->base.base.unk08 == 10) {
        legIndex = 0;
        if (variant == 0) {
            legIndex = 2;
        }
        if (foot == 3) {
            legIndex = legIndex + 1;
        }
        leg = self->legs[legIndex];
        g_btlFootPos[0] = leg->anchorPos[0];
        g_btlFootPos[1] = leg->anchorPos[1];
        g_btlFootPos[2] = leg->anchorPos[2];
        g_btlFootPos[3] = leg->anchorPos[3];
    } else {
        if (foot == 2) {
            GfxModelGetNodeWorldPos(&self->base, &bonePos, "Bip01_L_Foot");
        } else {
            GfxModelGetNodeWorldPos(&self->base, &bonePos, "Bip01_R_Foot");
        }
        g_btlFootPos[0] = bonePos.x;
        g_btlFootPos[1] = bonePos.y;
        g_btlFootPos[2] = bonePos.z;
        g_btlFootPos[3] = bonePos.w;
    }
    return g_btlFootPos;
}
