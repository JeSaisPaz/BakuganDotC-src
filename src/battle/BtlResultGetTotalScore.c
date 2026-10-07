// bdc 0x0883f5c8 BtlResultGetTotalScore
#include "bdc.h"

/* Rated-result score item 0x1e: `BtlResultGetBattleScore` plus the profile's point balance
   (`SaveProfileData` `points`, only when a profile exists), clamped to 0..9,999,999. */
int BtlResultGetTotalScore(void *hud)
{
    int total = BtlResultGetBattleScore(hud);

    if (SaveHasProfile()) {
        SaveProfile *profile = SaveGetProfile();
        total += profile->data->points;
    }
    if (total < 0) {
        return 0;
    }
    if (total > 9999999) {
        return 9999999;
    }
    return total;
}
