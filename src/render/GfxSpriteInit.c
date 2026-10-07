// bdc 0x089f3b30 GfxSpriteInit
#include "bdc.h"

/* Resets a `GfxSprite` to defaults: identity model matrix (`+0x20`), `tint`/`alpha` from
   `g_colorWhite` (`+0xb0`), zero `addColor` (`+0xc0`) and base size `width..maybe_sizeW` (`+0x70`),
   flags `+0xd0` = 1, no texture, blend 1, quad mode 6, stencil words `0xdcff0001`/`0xdd000000`
   (`+0x12c`/`+0x130`), `layerMask` 1 and the default quad template `g_gfxSpriteDefaultQuad`
   (`+0x120`). The zero vectors are the VFPU bank constant C720 = (0, 0, 0, 0). */

void GfxSpriteInit(GfxSprite *sprite)
{
    int i;

    for (i = 0; i < 16; i++) {
        sprite->matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
    sprite->tint[0] = g_colorWhite.x;
    sprite->tint[1] = g_colorWhite.y;
    sprite->tint[2] = g_colorWhite.z;
    sprite->alpha = g_colorWhite.w;
    sprite->addColor[0] = 0.0f;
    sprite->addColor[1] = 0.0f;
    sprite->addColor[2] = 0.0f;
    sprite->addColor[3] = 0.0f;
    sprite->flags = 1;
    sprite->texture = (void *)0x0;
    sprite->blendMode = 1;
    sprite->preDrawCallback = (void *)0x0;
    sprite->quadMode = 6;
    sprite->textureSlot = 0;
    sprite->layerMask = 1;
    sprite->geStencilTest = 0xdcff0001;
    sprite->geStencilOp = 0xdd000000;
    sprite->alphaRef = 0;
    sprite->width = 0.0f;
    sprite->height = 0.0f;
    sprite->depth = 0.0f;
    sprite->maybe_sizeW = 0.0f;
    sprite->vertices = (GfxSpriteVertex *)&g_gfxSpriteDefaultQuad;
}
