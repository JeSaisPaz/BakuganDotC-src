// bdc 0x0883f54c BtlResultGetBattleScore
#include "bdc.h"

/* Rated-result score item 0x1d: items 0x10 + 0x13 + 0x16 - 0x1c (clear bonus, combo bonus,
   HP bonus, penalty), floored at 0. Items are fetched in the order 0x1c, 0x16, 0x10, 0x13. */

int BtlResultGetBattleScore(void *hud)
{
    u32 penalty = BtlResultGetScoreItem(hud, 0x1c);
    u32 hpBonus = BtlResultGetScoreItem(hud, 0x16);
    u32 clearBonus = BtlResultGetScoreItem(hud, 0x10);
    u32 comboBonus = BtlResultGetScoreItem(hud, 0x13);
    int score = (int)(clearBonus + comboBonus + hpBonus - penalty);

    if (score < 0) {
        score = 0;
    }
    return score;
}
