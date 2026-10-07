// bdc 0x088406dc BtlResultScrollStripes
#include "bdc.h"

/* Per-frame scroll of the rated-result background (`BtlHudRatingResultScreen`): moves the 2×41
   stripe sprites (rating sprites 93..133 and 134..174) up 2 px, adding 320 to any whose Y drops
   below -24, and the floating sprite 85 up 2.5 px; once its Y drops below -144 it moves down by
   432 + rand[0,300) and gets a random X: 8 + rand[0,28) (left edge) or 420 + rand[0,28) (right
   edge), picked by the top bit of a second random. Randoms are `vrndi.s` words, lifted to
   PlatformRandU32. */
void BtlResultScrollStripes(void *hud)
{
    BtlHud *self = (BtlHud *)hud;
    GfxSprite *sprite;
    s32 i;

    for (i = 0; i < 41; i++) {
        sprite = self->ratingSprites[93 + i];
        sprite->posY = sprite->posY - 2.0f;
        if (sprite->posY < -24.0f) {
            sprite->posY = sprite->posY + 320.0f;
        }
        sprite = self->ratingSprites[134 + i];
        sprite->posY = sprite->posY - 2.0f;
        if (sprite->posY < -24.0f) {
            sprite->posY = sprite->posY + 320.0f;
        }
    }

    sprite = self->ratingSprites[85];
    sprite->posY = sprite->posY - 2.5f;
    if (sprite->posY < -144.0f) {
        s32 offset = (s32)(((PlatformRandU32() >> 16) * 300) >> 16);
        sprite->posY = ((float)offset + 432.0f) + sprite->posY;
        if ((((PlatformRandU32() >> 16) * 2) >> 16) != 0) {
            sprite->posX = (float)(s32)((((PlatformRandU32() >> 16) * 28) >> 16) + 8);
        } else {
            sprite->posX = (float)(s32)((((PlatformRandU32() >> 16) * 28) >> 16) + 420);
        }
    }
}
