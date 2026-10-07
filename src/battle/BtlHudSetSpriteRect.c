// bdc 0x0882c488 BtlHudSetSpriteRect
#include "bdc.h"

/* HUD helper: if `sprite` is not NULL, sets its texture rectangle to (u0, v0, u1, v1) with
   UiSpriteSetUvRect and its size to (u1 - u0, v1 - v0) with UiSpriteSetSize. `self` is unused. */

void BtlHudSetSpriteRect(float u0, float v0, float u1, float v1, BtlHud *self, GfxSprite *sprite)
{
    float rect[4];

    (void)self;
    rect[0] = u0;
    rect[1] = v0;
    rect[2] = u1;
    rect[3] = v1;
    if (sprite != NULL) {
        UiSpriteSetUvRect(sprite, rect);
        UiSpriteSetSize(u1 - u0, v1 - v0, sprite);
    }
}
