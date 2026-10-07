// bdc 0x08842614 BtlArenaResultAlignNumber
#include "bdc.h"

/* Arena result screen: right-aligns the 3-digit number of record item `item` (0..15): reads its
   value (`BtlResultGetScoreItem`) and, when it is below 100, moves each of the item's 3 digit
   sprites (`g_btlArenaResultDigitSprites``[item]`, indices into `arenaSprites`) left by lowering
   its rest X position (kept in `scaleX`, see `BtlArenaResultOffsetRow`) by 6, or by 12 when the
   value is below 10. The value is compared as signed. The second argument (the caller passes
   `arenaSprites`) is unused. */

void BtlArenaResultAlignNumber(BtlHud *self, GfxSprite **ignoredSprites, int item)
{
    s32 value;
    s32 digitSprites[16][3];
    int i;

    (void)ignoredSprites;
    value = (s32)BtlResultGetScoreItem(self, item);
    memcpy(digitSprites, g_btlArenaResultDigitSprites, sizeof(digitSprites));
    for (i = 0; i < 3; i++) {
        GfxSprite *sprite = self->arenaSprites[digitSprites[item][i]];

        if (value < 100) {
            if (value < 10) {
                sprite->scaleX = sprite->scaleX - 12.0f;
            } else {
                sprite->scaleX = sprite->scaleX - 6.0f;
            }
        }
    }
}
