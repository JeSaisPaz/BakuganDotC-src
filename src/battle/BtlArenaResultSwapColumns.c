// bdc 0x088426e0 BtlArenaResultSwapColumns
#include "bdc.h"

/* Arena result screen (`BtlHudArenaResultScreen`): for each row r = 0..3 swaps the `scaleY` of
   arena sprites 32 + 4r and 33 + 4r, and of the three sprite pairs 58 + 12r + i and 61 + 12r + i
   (i = 0..2), in `BtlHud` `arenaSprites`. */
void BtlArenaResultSwapColumns(void *hud)
{
    BtlHud *self = (BtlHud *)hud;
    s32 row;
    s32 i;
    float tmp;

    for (row = 0; row < 4; row++) {
        GfxSprite *a = self->arenaSprites[32 + row * 4];
        GfxSprite *b = self->arenaSprites[33 + row * 4];

        tmp = a->scaleY;
        a->scaleY = b->scaleY;
        self->arenaSprites[33 + row * 4]->scaleY = tmp;
        for (i = 0; i < 3; i++) {
            a = self->arenaSprites[58 + row * 12 + i];
            b = self->arenaSprites[61 + row * 12 + i];
            tmp = a->scaleY;
            a->scaleY = b->scaleY;
            self->arenaSprites[61 + row * 12 + i]->scaleY = tmp;
        }
    }
}
