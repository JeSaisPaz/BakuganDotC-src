// bdc 0x0882a370 BtlSwordBlurUpdate
#include "bdc.h"

/* Per-frame update of the `BtlSwordBlur` trail (called by `BtlBakuganUpdate`): on first use
   sets the six function-local static blade offsets (tip/base for Nemus kind 8, Percival EN kind
   0x12, Nemus EN kind 0x14), multiplies the owner model's root matrix by the weapon bone's local
   matrix, transforms the owner kind's tip and base offsets by it (w = 1) and pushes the segment
   (`BtlSwordBlurPushSegment`). The alpha `color[3]` is 0.85 while `active` is set, otherwise it fades by 0.2 per
   frame down to 0. For any other owner kind the binary has no offsets and reads stale registers
   as the vector addresses (UB below). */
void BtlSwordBlurUpdate(BtlSwordBlur *blur)
{
    float tipOut[4];
    float baseOut[4];
    float mtx[16];
    const float *root;
    int c;
    int r;
    const GmoNode *bone = blur->bone;
    const float *tip;
    const float *base;
    u32 kind;
    float alpha;

    if (g_btlSwordBlurNemusTipInit == 0) {
        g_btlSwordBlurNemusTipInit = 1;
        g_btlSwordBlurNemusTip[0] = -30.0f;
        g_btlSwordBlurNemusTip[1] = 0.0f;
        g_btlSwordBlurNemusTip[2] = 0.0f;
        g_btlSwordBlurNemusTip[3] = 0.0f;
    }
    if (g_btlSwordBlurNemusBaseInit == 0) {
        g_btlSwordBlurNemusBaseInit = 1;
        g_btlSwordBlurNemusBase[0] = 140.0f;
        g_btlSwordBlurNemusBase[1] = 0.0f;
        g_btlSwordBlurNemusBase[2] = 0.0f;
        g_btlSwordBlurNemusBase[3] = 0.0f;
    }
    if (g_btlSwordBlurPercivalTipInit == 0) {
        g_btlSwordBlurPercivalTipInit = 1;
        g_btlSwordBlurPercivalTip[0] = 0.0f;
        g_btlSwordBlurPercivalTip[1] = 0.0f;
        g_btlSwordBlurPercivalTip[2] = -3.2f;
        g_btlSwordBlurPercivalTip[3] = 0.0f;
    }
    if (g_btlSwordBlurPercivalBaseInit == 0) {
        g_btlSwordBlurPercivalBaseInit = 1;
        g_btlSwordBlurPercivalBase[0] = 100.0f;
        g_btlSwordBlurPercivalBase[1] = 0.0f;
        g_btlSwordBlurPercivalBase[2] = -3.2f;
        g_btlSwordBlurPercivalBase[3] = 0.0f;
    }
    if (g_btlSwordBlurNemusEnTipInit == 0) {
        g_btlSwordBlurNemusEnTipInit = 1;
        g_btlSwordBlurNemusEnTip[0] = -30.0f;
        g_btlSwordBlurNemusEnTip[1] = 0.0f;
        g_btlSwordBlurNemusEnTip[2] = 0.0f;
        g_btlSwordBlurNemusEnTip[3] = 0.0f;
    }
    if (g_btlSwordBlurNemusEnBaseInit == 0) {
        g_btlSwordBlurNemusEnBaseInit = 1;
        g_btlSwordBlurNemusEnBase[0] = 120.0f;
        g_btlSwordBlurNemusEnBase[1] = 0.0f;
        g_btlSwordBlurNemusEnBase[2] = 0.0f;
        g_btlSwordBlurNemusEnBase[3] = 0.0f;
    }

    /* mtx = owner root matrix * bone local matrix (vmmul.q M000, M100, M200, rows of each matrix
       loaded as VFPU columns, column-major product): mtx[c][r] = sum_k root[k][r] * bone[c][k]. */
    root = blur->owner->base.data->rootMatrix;
    for (c = 0; c < 4; c++) {
        for (r = 0; r < 4; r++) {
            mtx[c * 4 + r] = VfDot4(root[0 * 4 + r], root[1 * 4 + r], root[2 * 4 + r], root[3 * 4 + r], bone->localMatrix[c * 4 + 0], bone->localMatrix[c * 4 + 1], bone->localMatrix[c * 4 + 2], bone->localMatrix[c * 4 + 3]);
        }
    }

    kind = blur->owner->base.base.unk08;
    if (kind == 0x14) {
        tip = g_btlSwordBlurNemusEnTip;
        base = g_btlSwordBlurNemusEnBase;
    } else if (kind == 0x12) {
        tip = g_btlSwordBlurPercivalTip;
        base = g_btlSwordBlurPercivalBase;
    } else if (kind == 8) {
        tip = g_btlSwordBlurNemusTip;
        base = g_btlSwordBlurNemusBase;
    } else {
        /* UB (original binary): no offsets for this kind. a2 still holds the compare constant 8
           and a1 the kind, and lv.q reads them as the tip and base vector addresses (address 8
           is unmapped and not 16-byte aligned). Unreachable while only kinds 8/0x12/0x14 get a
           trail (BtlBakuganCreateAttachments). */
        const float *ub_a2 = (const float *)(uintptr_t)8;
        const float *ub_a1 = (const float *)(uintptr_t)kind;
        tip = ub_a2;
        base = ub_a1;
    }

    /* tipOut/baseOut = mtx * (offset.xyz, 1) (vtfm4.q E100 form, w lane set to 1 by vfim). */
    for (r = 0; r < 4; r++) {
        tipOut[r] = VfDot4(mtx[0 + r], mtx[4 + r], mtx[8 + r], mtx[12 + r], tip[0], tip[1], tip[2], 1.0f);
    }
    for (r = 0; r < 4; r++) {
        baseOut[r] = VfDot4(mtx[0 + r], mtx[4 + r], mtx[8 + r], mtx[12 + r], base[0], base[1], base[2], 1.0f);
    }
    BtlSwordBlurPushSegment(blur, tipOut, baseOut);

    if (blur->active != 0) {
        blur->color[3] = 0.85f;
        return;
    }
    alpha = blur->color[3] - 0.2f;
    blur->color[3] = alpha;
    if (alpha < 0.0f) {
        blur->color[3] = 0.0f;
    }
}
