// bdc 0x089f4bc4 GfxSpriteSetVCell
#include "bdc.h"

/* Selects vertical cell `cell` of a sprite sheet: v0 = `height * cell * texFactor`, v1 = `height *
   (cell + 1) * texFactor` (texture factor `tex+0xa8`). No-op for template modes 0/1. */

void GfxSpriteSetVCell(float cell, GfxSprite *sprite)
{
    GfxSpriteVertex(*quad)[4];
    float factor;
    float val;

    factor = ((GfxTexture *)sprite->texture)->invHeight;
    if (sprite->quadMode >= 0 && sprite->quadMode < 2) {
        return;
    }
    quad = (GfxSpriteVertex(*)[4])sprite->vertices;
    val = sprite->height * cell * factor;
    (*quad)[1].v = val;
    (*quad)[0].v = val;
    quad = (GfxSpriteVertex(*)[4])sprite->vertices;
    val = sprite->height * (cell + 1.0f) * factor;
    (*quad)[3].v = val;
    (*quad)[2].v = val;
}
