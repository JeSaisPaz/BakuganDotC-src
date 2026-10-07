// bdc 0x08835c24 BtlHudShowGuideCaption
#include "bdc.h"

/* Shows the caption of button-guide page `page`: HUD sprite 158 gets alpha 0, is made visible,
   placed at `(x, 144)`, its scale/angle quad (`scaleX..angle`) zeroed (VFPU bank C720 = 0), and its
   vertical cell set to the page's `captionCell` (`g_btlHudGuidePages`, `GfxSpriteSetVCell`).
   The four outline copies (sprites 247..250) take the same position, visibility, alpha 0,
   zeroed scale/angle quad and cell, then are offset by one pixel: -1/+1 in X (copies 0/1), then
   -1/+1 in Y (copies 2/3). */
void BtlHudShowGuideCaption(float x, BtlHud *self, s32 page)
{
    GfxSprite *caption;
    GfxSprite *copy;
    s32 i;

    self->sprites[158]->alpha = 0.0f;
    self->sprites[158]->flags |= 1;
    self->sprites[158]->posX = x;
    self->sprites[158]->posY = 144.0f;
    caption = self->sprites[158];
    /* scaleX..angle = bank C720 (0, 0, 0, 0), one quad store */
    caption->scaleX = 0.0f;
    caption->scaleY = 0.0f;
    caption->scaleZ = 0.0f;
    caption->angle = 0.0f;
    GfxSpriteSetVCell((float)g_btlHudGuidePages[page].captionCell, self->sprites[158]);
    for (i = 0; i < 4; i++) {
        copy = self->sprites[247 + i];
        copy->posX = self->sprites[158]->posX;
        copy->posY = self->sprites[158]->posY;
        copy->flags |= 1;
        copy->alpha = 0.0f;
        copy->scaleX = 0.0f;
        copy->scaleY = 0.0f;
        copy->scaleZ = 0.0f;
        copy->angle = 0.0f;
        GfxSpriteSetVCell((float)g_btlHudGuidePages[page].captionCell, copy);
        if (i < 2) {
            if (i % 2 != 0) {
                copy->posX = copy->posX + 1.0f;
            } else {
                copy->posX = copy->posX - 1.0f;
            }
        } else {
            if (i % 2 != 0) {
                copy->posY = copy->posY + 1.0f;
            } else {
                copy->posY = copy->posY - 1.0f;
            }
        }
    }
}
