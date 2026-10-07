// bdc 0x089f4b34 GfxSpriteSetUCell
#include "bdc.h"

/* Selects horizontal cell `cell` of a sprite sheet: u0 = `width * cell * texFactor`, u1 = `width *
   (cell + 1) * texFactor` (texture factor `tex+0xa4`). No-op for template modes 0/1. */

void GfxSpriteSetUCell(float cell, GfxSprite *sprite)
{
    GfxSpriteVertex(*quad)[4];
    float factor;
    float val;

    factor = ((GfxTexture *)sprite->texture)->invWidth;
    if (sprite->quadMode >= 0 && sprite->quadMode < 2) {
        return;
    }
    quad = (GfxSpriteVertex(*)[4])sprite->vertices;
    val = sprite->width * cell * factor;
    (*quad)[2].u = val;
    (*quad)[0].u = val;
    quad = (GfxSpriteVertex(*)[4])sprite->vertices;
    val = sprite->width * (cell + 1.0f) * factor;
    (*quad)[3].u = val;
    (*quad)[1].u = val;
}
