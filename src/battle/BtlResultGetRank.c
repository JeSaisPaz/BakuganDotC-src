// bdc 0x0883fb00 BtlResultGetRank
#include "bdc.h"

/* Rank (0 = S, 1 = A, 2 = B, 3 = C; the `"hyouka_moji_02_s/a/b/c"` letters) of rated-result
   category `category` from `BtlResultGetScoreItem` (values compared signed): 0 combo bonus
   (item 0x13: >=1000 S, >=800 A, >=300 B), 1 HP bonus (item 0x16: >=1500/1000/500), 2 penalty
   (item 0x1c, inverted: <200 S, <400 A, <700 B), 3 overall score combo + HP - penalty
   (>=2300/1400/500). Any other category returns 0. */

int BtlResultGetRank(void *hud, int category)
{
    s32 score;
    s32 penalty;
    u32 penaltyRaw;
    u32 comboRaw;
    s32 total;

    switch (category) {
    case 0:
        score = (s32)BtlResultGetScoreItem(hud, 0x13);
        if (score < 300) {
            return 3;
        }
        if (score < 800) {
            return 2;
        }
        if (score < 1000) {
            return 1;
        }
        return 0;
    case 1:
        score = (s32)BtlResultGetScoreItem(hud, 0x16);
        if (score < 500) {
            return 3;
        }
        if (score < 1000) {
            return 2;
        }
        if (score < 1500) {
            return 1;
        }
        return 0;
    case 2:
        penalty = (s32)BtlResultGetScoreItem(hud, 0x1c);
        if (penalty < 200) {
            return 0;
        }
        if (penalty < 400) {
            return 1;
        }
        if (penalty < 700) {
            return 2;
        }
        return 3;
    case 3:
        penaltyRaw = BtlResultGetScoreItem(hud, 0x1c);
        comboRaw = BtlResultGetScoreItem(hud, 0x13);
        total = (s32)(comboRaw + BtlResultGetScoreItem(hud, 0x16) - penaltyRaw); /* wrapping */
        if (total < 500) {
            return 3;
        }
        if (total < 1400) {
            return 2;
        }
        if (total < 2300) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}
