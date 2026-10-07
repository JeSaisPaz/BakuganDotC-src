// bdc 0x0883f380 BtlResultGetClearBonus
#include "bdc.h"

/* Rated-result score item 0x10 (`BtlResultGetScoreItem`): the battle-clear bonus for the current
   stage. Copies the 41-entry `g_btlClearBonusTable` to the stack and, when `g_btlBattleOutcome` is
   1 (won), returns the entry for the stage number (script global entry 1 of `g_scriptGlobalVars`,
   clamped to 0..0x26): doubled when the profile's `playthrough` count is non-zero, otherwise with
   30000 added for stage 0x18. Returns 0 for any other outcome. `hud` is unused. */

int BtlResultGetClearBonus(void *hud)
{
    s32 table[41];
    s32 stage;
    int bonus;
    SaveProfile *profile;

    (void)hud;
    memcpy(table, g_btlClearBonusTable, sizeof(table));
    stage = g_scriptGlobalVars[1];
    if (stage < 0) {
        stage = 0;
    } else if (stage > 0x26) {
        stage = 0x26;
    }
    bonus = 0;
    if (g_btlBattleOutcome == 1) {
        profile = (SaveProfile *)SaveGetProfile();
        bonus = table[stage];
        if (profile->data->playthrough != 0) {
            bonus = bonus * 2;
        } else if (stage == 0x18) {
            bonus = bonus + 30000;
        }
    }
    return bonus;
}
