// bdc 0x089f4564 GfxSpriteSetTopLeftPivot
#include "bdc.h"

/* Converts a sprite with a centred (quad modes 1/3) or bottom-centre (mode 4) pivot to top-left
   mode 2 without moving it on screen: shifts `posX`/`posY` back by half the size (full height for
   mode 4), copies the vertex XY bytes from `g_gfxQuadTemplateTopLeft` into `vertexBuf` and points
   `vertices` at it. Modes 0, 2 and >= 5 are left untouched. */

void GfxSpriteSetTopLeftPivot(GfxSprite *sprite)
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
            changed = true;
            x = sprite->posX;
            w = GfxSpriteGetWidth(sprite);
            y = sprite->posY;
            sprite->posX = x - w * 0.5f;
            h = GfxSpriteGetHeight(sprite);
            sprite->posY = y - h * 0.5f;
        } else if (mode == 4) {
            changed = true;
            x = sprite->posX;
            w = GfxSpriteGetWidth(sprite);
            sprite->posX = x - w * 0.5f;
            h = GfxSpriteGetHeight(sprite);
            sprite->posY = sprite->posY - h;
        }
    }
    if (changed) {
        for (i = 0; i < 4; i++) {
            sprite->vertexBuf[i].x = g_gfxQuadTemplateTopLeft[i].x;
            sprite->vertexBuf[i].y = g_gfxQuadTemplateTopLeft[i].y;
        }
        sprite->vertices = sprite->vertexBuf;
        sprite->quadMode = 2;
    }
}
