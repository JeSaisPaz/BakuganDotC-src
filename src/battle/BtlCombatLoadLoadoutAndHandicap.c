// bdc 0x08886c54 BtlCombatLoadLoadoutAndHandicap
#include "bdc.h"

/* Loads a `BtlCombatState`'s profile loadout (`BtlCombatLoadLoadout`) and, when script global
   variable 8 (`g_scriptGlobalVars`, battle rule mode) is 2, applies the owner's HP handicap:
   takes profile word `0x46 + playerSlot` of the owner unit (`SaveGetProfile` `words`; 0 when the
   word table is missing) and sets max and current HP (`BtlCombatFillHp`) to the stat table's
   `levelHp[4]`, halved for handicap 50 (0x32), ×1.5 for 150 (0x96), unchanged otherwise. */

void BtlCombatLoadLoadoutAndHandicap(BtlCombatState *combat)
{
    s32 playerSlot;
    SaveProfile *profile;
    s32 handicap;
    float hp;

    BtlCombatLoadLoadout(combat);
    if (g_scriptGlobalVars[8] != 2) {
        return;
    }
    playerSlot = ((BtlBakugan *)combat->owner)->playerSlot;
    profile = SaveGetProfile();
    handicap = 0;
    hp = combat->stats->levelHp[4];
    if (profile->words != NULL) {
        handicap = profile->words[0x46 + playerSlot];
    }
    if (handicap == 0x32) {
        hp = hp * 0.5f;
    } else if (handicap == 0x96) {
        hp = hp * 1.5f;
    }
    BtlCombatFillHp(combat, hp);
}
