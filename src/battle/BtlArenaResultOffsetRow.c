// bdc 0x08842874 BtlArenaResultOffsetRow
#include "bdc.h"

/* Arena result screen: for each of the 23 sprite slots of result row `row`
   (`g_btlArenaResultRowSprites`, -1 = none), sets the sprite's position X/Y to the rest position
   kept in its `scaleX`/`scaleY` words plus (`dx`, `dy`); an offset equal to 0 copies the rest value
   unchanged. For a `row` outside 0..3 the index from the previous slot (initially -1) is reused. */
void BtlArenaResultOffsetRow(float dx, float dy, BtlHud *hud, int row)
{
    s32 index = -1;
    int i;

    for (i = 0; i < 23; i++) {
        if (row >= 0 && row < 4) {
            index = g_btlArenaResultRowSprites[row][i];
        }
        if (index != -1) {
            GfxSprite *sprite = hud->arenaSprites[index];

            if (dx == 0.0f) {
                sprite->posX = sprite->scaleX;
            } else {
                sprite->posX = sprite->scaleX + dx;
            }
            if (dy == 0.0f) {
                sprite->posY = sprite->scaleY;
            } else {
                sprite->posY = sprite->scaleY + dy;
            }
        }
    }
}
