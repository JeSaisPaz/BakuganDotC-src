// bdc 0x089dce50 GmoDlWriteBlendMode
#include "bdc.h"

/* Writes the material's alpha blend: `0xdf` with the equation (`+0x10`) and the source/destination
   factors mapped through the table `0x08aa2f8d` (`+0x11`, `+0x12`), plus the fixed colours
   `0xe0`/`0xe1` (white when the factor is non-zero). */

void GmoDlWriteBlendMode(GmoDlContext *self)
{
    GmoAttr *mat = self->material;
    u32 op = mat->blendOp;
    u32 srcColor = 0xffffffff;
    u32 dstColor = 0xffffffff;
    s32 src;
    s32 dst;

    if (mat->blendSrc == 0) {
        srcColor = 0;
    }
    if (mat->blendDst == 0) {
        dstColor = 0;
    }
    src = g_gmoBlendFactorMap[mat->blendSrc];
    dst = g_gmoBlendFactorMap[mat->blendDst];
    *self->cur++ = (dst << 4) | src | (op << 8) | 0xdf000000;
    *self->cur++ = (srcColor & 0xffffff) | 0xe0000000;
    *self->cur++ = (dstColor & 0xffffff) | 0xe1000000;
}
