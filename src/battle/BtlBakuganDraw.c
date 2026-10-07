// bdc 0x0885f50c BtlBakuganDraw
#include "bdc.h"

/* `dst = a * b` for column-major 4x4 matrices (`vmmul.q M000, M100, M200` with `a` in M100 and `b`
   in M200): column j of `dst` = sum over k of b[j][k] * column k of `a`. Both inputs are read before
   `dst` is written, so `dst` may alias `a` or `b`. */
static void BtlBakuganDrawMatMul(float *dst, const float *a, const float *b)
{
    float r[16];
    int j;
    int i;

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            r[j * 4 + i] = b[j * 4 + 0] * a[0 * 4 + i] + b[j * 4 + 1] * a[1 * 4 + i]
                         + b[j * 4 + 2] * a[2 * 4 + i] + b[j * 4 + 3] * a[3 * 4 + i];
        }
    }
    for (i = 0; i < 16; i++) {
        dst[i] = r[i];
    }
}

/* 4x4 matrix copy (`lv.q`/`sv.q` through M000). */
static void BtlBakuganDrawMatCopy(float *dst, const float *src)
{
    int i;

    for (i = 0; i < 16; i++) {
        dst[i] = src[i];
    }
}

/* Calls the draw virtual (vtable slot 8, `+0x40`) of `model` with the draw context. */
static void BtlBakuganDrawCallDraw(GfxModel *model, void *ctx)
{
    const VtblEntry *entry = &((const VtblEntry *)model->base.vtable)[8];

    ((void (*)(void *, void *))entry->fn)((u8 *)model + entry->delta, ctx);
}

/* Draw virtual of the battle Bakugan (vtable `0x08af1fa4` slot `+0x40`): writes the model's GE state
   (`GfxModelDlWriteState`), fills the two foot attach matrices on first use, then draws the
   kind-specific attachments. Kind 7 (when `stateFlags & 0x1800000`): effect model 0
   (`g_gfxEffectModels[0]`) placed at `root * "Bip01_L_Foot" * g_btlLFootAttachMatrix` and drawn, then
   again at the right foot with `g_btlRFootAttachMatrix`. Kinds 8/0x12/0x14: the weapon's sword blur
   (`BtlSwordBlurDraw`) when `weapon` is set. Kind 0xe (when `stateFlags & 0x1800000`): the wing model
   (`wingModel`) placed at `root * "Bip01_L_Wing"` and drawn, then at `root * "Bip01_R_Wing"` mirrored
   by `g_gfxFlipYZMatrix` and drawn. Other kinds draw nothing more. */
void BtlBakuganDraw(BtlBakugan *self, void *ctx)
{
    float tmp[16];
    const float *root;
    float *dst;
    ScePspFMatrix4 *node;

    GfxModelDlWriteState(&self->base, ctx);

    if (g_btlLFootAttachMatrixInit == 0) {
        g_btlLFootAttachMatrixInit = 1;
        g_btlLFootAttachMatrix[0] = -0.378500015f;
        g_btlLFootAttachMatrix[1] = -0.0884765983f;
        g_btlLFootAttachMatrix[2] = -0.921362996f;
        g_btlLFootAttachMatrix[3] = 0.0f;
        g_btlLFootAttachMatrix[4] = -0.897283018f;
        g_btlLFootAttachMatrix[5] = -0.209267005f;
        g_btlLFootAttachMatrix[6] = 0.388704002f;
        g_btlLFootAttachMatrix[7] = 0.0f;
        g_btlLFootAttachMatrix[8] = -0.227201998f;
        g_btlLFootAttachMatrix[9] = 0.973847985f;
        g_btlLFootAttachMatrix[10] = -0.000180942996f;
        g_btlLFootAttachMatrix[11] = 0.0f;
        g_btlLFootAttachMatrix[12] = -10.0f;
        g_btlLFootAttachMatrix[13] = -6.0f;
        g_btlLFootAttachMatrix[14] = 0.0f;
        g_btlLFootAttachMatrix[15] = 1.0f;
    }
    if (g_btlRFootAttachMatrixInit == 0) {
        g_btlRFootAttachMatrixInit = 1;
        g_btlRFootAttachMatrix[0] = 0.379967004f;
        g_btlRFootAttachMatrix[1] = 0.0884765983f;
        g_btlRFootAttachMatrix[2] = -0.920759022f;
        g_btlRFootAttachMatrix[3] = 0.0f;
        g_btlRFootAttachMatrix[4] = -0.89666301f;
        g_btlRFootAttachMatrix[5] = -0.209267005f;
        g_btlRFootAttachMatrix[6] = -0.39013201f;
        g_btlRFootAttachMatrix[7] = 0.0f;
        g_btlRFootAttachMatrix[8] = -0.227201998f;
        g_btlRFootAttachMatrix[9] = 0.973847985f;
        g_btlRFootAttachMatrix[10] = -0.000180942996f;
        g_btlRFootAttachMatrix[11] = 0.0f;
        g_btlRFootAttachMatrix[12] = -10.0f;
        g_btlRFootAttachMatrix[13] = -6.0f;
        g_btlRFootAttachMatrix[14] = 0.0f;
        g_btlRFootAttachMatrix[15] = 1.0f;
    }

    switch (self->base.base.unk08) {
    case 7:
        if ((self->stateFlags & 0x1800000) != 0) {
            root = self->base.data->rootMatrix;
            node = GfxModelFindNodeMatrix(&self->base, "Bip01_L_Foot");
            BtlBakuganDrawMatMul(tmp, root, (const float *)node);
            BtlBakuganDrawMatMul(tmp, tmp, g_btlLFootAttachMatrix);
            BtlBakuganDrawMatCopy(g_gfxEffectModels[0]->data->rootMatrix, tmp);
            BtlBakuganDrawCallDraw(g_gfxEffectModels[0], ctx);

            root = self->base.data->rootMatrix;
            node = GfxModelFindNodeMatrix(&self->base, "Bip01_R_Foot");
            BtlBakuganDrawMatMul(tmp, root, (const float *)node);
            BtlBakuganDrawMatMul(tmp, tmp, g_btlRFootAttachMatrix);
            BtlBakuganDrawMatCopy(g_gfxEffectModels[0]->data->rootMatrix, tmp);
            BtlBakuganDrawCallDraw(g_gfxEffectModels[0], ctx);
        }
        break;
    case 8:
    case 0x12:
    case 0x14:
        if (self->weapon != NULL) {
            BtlSwordBlurDraw(self->weapon, ctx);
        }
        break;
    case 0xe:
        if ((self->stateFlags & 0x1800000) != 0) {
            dst = ((GfxModel *)self->wingModel)->data->rootMatrix;
            root = self->base.data->rootMatrix;
            node = GfxModelFindNodeMatrix(&self->base, "Bip01_L_Wing");
            BtlBakuganDrawMatMul(dst, root, (const float *)node);
            BtlBakuganDrawCallDraw((GfxModel *)self->wingModel, ctx);

            dst = ((GfxModel *)self->wingModel)->data->rootMatrix;
            root = self->base.data->rootMatrix;
            node = GfxModelFindNodeMatrix(&self->base, "Bip01_R_Wing");
            BtlBakuganDrawMatMul(dst, root, (const float *)node);
            dst = ((GfxModel *)self->wingModel)->data->rootMatrix;
            BtlBakuganDrawMatMul(dst, dst, (const float *)&g_gfxFlipYZMatrix);
            BtlBakuganDrawCallDraw((GfxModel *)self->wingModel, ctx);
        }
        break;
    default:
        break;
    }
}
