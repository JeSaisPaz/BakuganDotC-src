// bdc 0x08842970 BtlArenaResultInitScrollStripes
#include "bdc.h"

/* Arena-result twin of BtlResultInitScrollStripes, on the arena result layout: clones 2 x 41
   stripe sprites from templates arenaSprites[29]/[30] into arenaSprites[112..152]/[153..193]
   (Y from -24 in 8-px steps, Z + 4, random X scale 0.8..1.7 from the VFPU random generator
   vrndi.s, lifted to PlatformRandU32; hidden with alpha 0; the second set is also shifted left by (scale - 1) * 32),
   places the floating sprite arenaSprites[31] at a random position (X 420..447, Y 0..271),
   and pushes arenaSprites[28] back by 6 in Z with alpha 0. */

void BtlArenaResultInitScrollStripes(BtlHud *self)
{
    GfxSprite *sprite;
    GfxSprite *floating;
    float scaleX;
    u32 rnd;
    s32 i;
    s32 y;

    for (i = 0, y = -24; i < 41; i++, y += 8) {
        sprite = GfxSpriteLayerCloneSprite(self->arenaLayer, self->arenaSprites[29]);
        self->arenaSprites[112 + i] = sprite;
        sprite->posZ = sprite->posZ + 4.0f;
        sprite->posY = (float)y;
        rnd = PlatformRandU32();
        GfxSpriteSetScaleRotation(sprite, (float)((((rnd >> 16) * 10) >> 16) + 8) * 0.1f,
                                  1.0f, 0.0f, false);
        sprite->alpha = 0.0f;
        sprite->flags |= 1;
    }
    for (i = 0, y = -24; i < 41; i++, y += 8) {
        sprite = GfxSpriteLayerCloneSprite(self->arenaLayer, self->arenaSprites[30]);
        self->arenaSprites[153 + i] = sprite;
        sprite->posY = (float)y;
        sprite->posZ = sprite->posZ + 4.0f;
        rnd = PlatformRandU32();
        scaleX = (float)((((rnd >> 16) * 10) >> 16) + 8) * 0.1f;
        GfxSpriteSetScaleRotation(sprite, scaleX, 1.0f, 0.0f, false);
        sprite->posX = sprite->posX - (scaleX - 1.0f) * 32.0f;
        sprite->alpha = 0.0f;
        sprite->flags |= 1;
    }
    floating = self->arenaSprites[31];
    rnd = PlatformRandU32();
    floating->posY = (float)(((rnd >> 16) * 0x110) >> 16);
    rnd = PlatformRandU32();
    floating->posX = (float)((((rnd >> 16) * 0x1c) >> 16) + 0x1a4);
    floating->flags |= 1;
    floating->alpha = 0.0f;
    sprite = self->arenaSprites[28];
    sprite->posZ = sprite->posZ + 6.0f;
    sprite->alpha = 0.0f;
}
