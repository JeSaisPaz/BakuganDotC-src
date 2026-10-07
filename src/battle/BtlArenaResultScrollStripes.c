// bdc 0x08842d38 BtlArenaResultScrollStripes
#include "bdc.h"

/* Per-frame scroll of the arena-result background (`BtlHudArenaResultScreen`); arena twin of
   `BtlResultScrollStripes`. Moves the 2x41 stripe sprites (arenaSprites[112..152] and
   [153..193]) up 2 px, wrapping each by +320 once below -24, and the floating sprite
   arenaSprites[31] up 2.5 px; once that one is below -144 it moves down by 432 + rnd(300) and
   gets a random X in 8..35 or 420..447 (coin flip on the random word's top bit). */

void BtlArenaResultScrollStripes(BtlHud *self)
{
    GfxSprite *sprite;
    s32 i;

    for (i = 0; i < 41; i++) {
        sprite = self->arenaSprites[112 + i];
        sprite->posY = sprite->posY - 2.0f;
        if (sprite->posY < -24.0f) {
            sprite->posY = sprite->posY + 320.0f;
        }
        sprite = self->arenaSprites[153 + i];
        sprite->posY = sprite->posY - 2.0f;
        if (sprite->posY < -24.0f) {
            sprite->posY = sprite->posY + 320.0f;
        }
    }

    sprite = self->arenaSprites[31];
    sprite->posY = sprite->posY - 2.5f;
    if (sprite->posY < -144.0f) {
        s32 dy = (s32)(((PlatformRandU32() >> 16) * 300) >> 16);
        sprite->posY = ((float)dy + 432.0f) + sprite->posY;
        if ((((PlatformRandU32() >> 16) * 2) >> 16) != 0) {
            sprite->posX = (float)((s32)(((PlatformRandU32() >> 16) * 28) >> 16) + 8);
        } else {
            sprite->posX = (float)((s32)(((PlatformRandU32() >> 16) * 28) >> 16) + 420);
        }
    }
}
