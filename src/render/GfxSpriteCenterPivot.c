// bdc 0x089f46ac GfxSpriteCenterPivot
#include "bdc.h"

/* Converts a 2D sprite (0x160-byte quad object, init `GfxSpriteInit`) from a top-left or
   bottom-anchored quad to the centred quad mode 3, shifting its position so it stays visually in
   place: modes 0/2 move `pos += size/2`, mode 4 moves `posY -= height/2`. It then switches the
   sprite to its own vertex buffer (`vertices = vertexBuf`) and copies the centred corner XY bytes
   from `g_gfxQuadTemplateCentre`. Modes 1, 3 and >= 5 are left untouched. */

void GfxSpriteCenterPivot(GfxSprite *sprite)
{
    u32 mode = (u32)sprite->quadMode;
    bool changed = false;
    float x;
    float y;
    float w;
    float h;
    int i;

    if (mode < 5) {
        if (mode == 1 || mode == 3) {
            /* already centred */
        } else if (mode == 4) {
            changed = true;
            y = sprite->posY;
            h = GfxSpriteGetHeight(sprite);
            sprite->posY = y - h * 0.5f;
        } else {
            changed = true;
            x = sprite->posX;
            w = GfxSpriteGetWidth(sprite);
            y = sprite->posY;
            sprite->posX = x + w * 0.5f;
            h = GfxSpriteGetHeight(sprite);
            sprite->posY = y + h * 0.5f;
        }
    }
    if (changed) {
        for (i = 0; i < 4; i++) {
            sprite->vertexBuf[i].x = g_gfxQuadTemplateCentre[i].x;
            sprite->vertexBuf[i].y = g_gfxQuadTemplateCentre[i].y;
        }
        sprite->vertices = sprite->vertexBuf;
        sprite->quadMode = 3;
    }
}
