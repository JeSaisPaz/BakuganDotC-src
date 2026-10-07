// bdc 0x08842c5c BtlArenaResultSetAlpha
#include "bdc.h"

/* Arena result screen (`BtlHudArenaResultScreen`): sets `alpha` on every sprite of the 4 result
   rows, i.e. each `arenaSprites` entry listed in `g_btlArenaResultRowSprites` (slots holding -1
   are skipped). */
void BtlArenaResultSetAlpha(float alpha, BtlHud *hud)
{
    s32 row;
    s32 slot;
    s32 index;

    for (row = 0; row < 4; row++) {
        for (slot = 0; slot < 23; slot++) {
            index = g_btlArenaResultRowSprites[row][slot];
            if (index != -1) {
                hud->arenaSprites[index]->alpha = alpha;
            }
        }
    }
}
