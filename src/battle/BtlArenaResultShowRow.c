// bdc 0x08842798 BtlArenaResultShowRow
#include "bdc.h"

/* Arena result screen (`BtlHudArenaResultScreen`): sets (`show`) or clears bit 0 (visible) of the
   flags of the 23 arena layout sprites listed for result row `row` in
   `g_btlArenaResultRowSprites`, skipping -1 entries. For a `row` outside 0..3 the index stays at
   its initial -1, so nothing changes. */
void BtlArenaResultShowRow(BtlHud *hud, int row, bool show)
{
    s32 index = -1;
    int i;

    for (i = 0; i < 23; i++) {
        if (row >= 0 && row < 4) {
            index = g_btlArenaResultRowSprites[row][i];
        }
        if (index != -1) {
            GfxSprite *sprite = hud->arenaSprites[index];

            if (show) {
                sprite->flags |= 1;
            } else {
                sprite->flags &= ~1u;
            }
        }
    }
}
