// bdc 0x088401a8 BtlResultInitScrollStripes
#include "bdc.h"

/* Rated-result screen (`BtlHudRatingResultScreen`) background setup: clones 41 stripe sprites
   from rating sprite 0x53 and 41 from sprite 0x54 into ratingSprites[0x5d..] / [0x86..], at
   Y = -24 + 8*i, Z + 4, random X scale 0.8..1.7 (the second set also shifts X left by
   (scale - 1) * 32), alpha 0, flags bit 0 set; then puts sprite 0x55 at a random X 420..447,
   Y 0..271 and moves sprite 0x52 6 deeper in Z with alpha 0. The u32 -> float conversions are
   the binary's unsigned ones (cvt.s.w plus 2^32 when negative). Randoms are `vrndi.s`
   words, lifted to PlatformRandU32. */
void BtlResultInitScrollStripes(BtlHud *self)
{
    GfxSprite *sprite;
    float scaleX;
    int i;
    int y;
    u32 n;

    for (i = 0, y = -24; i < 41; i++, y += 8) {
        sprite = GfxSpriteLayerCloneSprite(self->ratingLayer, self->ratingSprites[0x53]);
        self->ratingSprites[0x5d + i] = sprite;
        sprite->posZ = sprite->posZ + 4.0f;
        sprite->posY = (float)y;
        n = ((PlatformRandU32() >> 16) * 10 >> 16) + 8;
        GfxSpriteSetScaleRotation(sprite, (float)n * 0.100000001f, 1.0f, 0.0f, false);
        sprite->alpha = 0.0f;
        sprite->flags |= 1;
    }
    for (i = 0, y = -24; i < 41; i++, y += 8) {
        sprite = GfxSpriteLayerCloneSprite(self->ratingLayer, self->ratingSprites[0x54]);
        self->ratingSprites[0x86 + i] = sprite;
        sprite->posY = (float)y;
        sprite->posZ = sprite->posZ + 4.0f;
        n = ((PlatformRandU32() >> 16) * 10 >> 16) + 8;
        scaleX = (float)n * 0.100000001f;
        GfxSpriteSetScaleRotation(sprite, scaleX, 1.0f, 0.0f, false);
        sprite->posX = sprite->posX - (scaleX - 1.0f) * 32.0f;
        sprite->alpha = 0.0f;
        sprite->flags |= 1;
    }
    sprite = self->ratingSprites[0x55];
    n = (PlatformRandU32() >> 16) * 0x110 >> 16;
    sprite->posY = (float)n;
    n = ((PlatformRandU32() >> 16) * 0x1c >> 16) + 0x1a4;
    sprite->posX = (float)n;
    sprite->flags |= 1;
    sprite->alpha = 0.0f;
    sprite = self->ratingSprites[0x52];
    sprite->posZ = sprite->posZ + 6.0f;
    sprite->alpha = 0.0f;
}
