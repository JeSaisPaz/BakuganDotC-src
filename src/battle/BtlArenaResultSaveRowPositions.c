// bdc 0x0884250c BtlArenaResultSaveRowPositions
#include "bdc.h"

/* Arena result screen (`BtlHudArenaResultScreen`): for each of the 4 result rows and each of
   its 23 entries in `g_btlArenaResultRowSprites` (-1 = none), copies the arena layout sprite's
   position quad (`posX..posW`) into its `scaleX..angle` quad (an `lv.q`/`sv.q` 16-byte copy);
   entry 0x16 of each row only when `all` is set. */
void BtlArenaResultSaveRowPositions(void *hud, bool all)
{
    BtlHud *self = (BtlHud *)hud;
    int row;
    int i;

    for (row = 0; row < 4; row++) {
        for (i = 0; i < 23; i++) {
            s32 index = g_btlArenaResultRowSprites[row][i];

            if (index != -1) {
                GfxSprite *sprite = self->arenaSprites[index];

                if (all || i != 0x16) {
                    sprite->scaleX = sprite->posX;
                    sprite->scaleY = sprite->posY;
                    sprite->scaleZ = sprite->posZ;
                    sprite->angle = sprite->posW;
                }
            }
        }
    }
}
