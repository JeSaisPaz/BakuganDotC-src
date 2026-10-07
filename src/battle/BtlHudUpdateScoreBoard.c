// bdc 0x08833ddc BtlHudUpdateScoreBoard
#include "bdc.h"

/* HUD widget updater (`BtlHudPhaseMain`) for the score board: runs the gain popup of each of
   the 4 players (`BtlHudUpdateScoreGainPopup`), then, when the battle rule mode (script
   global variable 8) is 2 and profile word 7 (score mode) is 1 or 2, redraws score counters
   0..8 with their current values (`BtlHudRefreshScoreCounter` with -999). */

void BtlHudUpdateScoreBoard(BtlHud *self)
{
    s32 scoreMode;
    int i;

    for (i = 0; i < 4; i++) {
        BtlHudUpdateScoreGainPopup(self, i);
    }
    if (g_scriptGlobalVars[8] == 2) {
        scoreMode = (s32)SaveProfileGetWord((SaveProfile *)SaveGetProfile(), 7);
        if (scoreMode > 0 && scoreMode < 3) {
            for (i = 0; i < 9; i++) {
                BtlHudRefreshScoreCounter(self, i, -999);
            }
        }
    }
}
