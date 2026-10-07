// bdc 0x089f47dc GfxSpriteSetBottomCentrePivot
#include "bdc.h"

/* Converts a sprite to bottom-centre quad mode 4 without moving it on screen: from centred modes
   1/3 it adds half the height to `posY`; from top-left modes (0/2) it adds half the width to `posX`
   and the full height to `posY`. Rewrites the vertex XY bytes from the mode-4 template
   `g_gfxQuadTemplateBottomCentre` and points `vertices` at `vertexBuf`. No-op for mode 4 and
   modes >= 5. */

void GfxSpriteSetBottomCentrePivot(GfxSprite *sprite)
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
            y = sprite->posY;
            h = GfxSpriteGetHeight(sprite);
            sprite->posY = y + h * 0.5f;
        } else if (mode != 4) {
            changed = true;
            x = sprite->posX;
            w = GfxSpriteGetWidth(sprite);
            sprite->posX = x + w * 0.5f;
            h = GfxSpriteGetHeight(sprite);
            sprite->posY = sprite->posY + h;
        }
    }
    if (changed) {
        for (i = 0; i < 4; i++) {
            sprite->vertexBuf[i].x = g_gfxQuadTemplateBottomCentre[i].x;
            sprite->vertexBuf[i].y = g_gfxQuadTemplateBottomCentre[i].y;
        }
        sprite->vertices = sprite->vertexBuf;
        sprite->quadMode = 4;
    }
}
