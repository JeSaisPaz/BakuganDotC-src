// bdc 0x08886d28 BtlCombatComputeMaxHp
#include "bdc.h"

/* Computes a unit's maximum hit points from its `BtlCombatState` stat table. Without an owner
   it is `stats->levelHp[level]`. In battle rule mode 2 (`g_scriptGlobalVars` entry 8) it is the
   level-4 value `levelHp[4]`, halved when the profile word 0x46 + owner `playerSlot`
   (`SaveGetProfile`; 0 without a word table) is 50 and multiplied by 1.5 when it is 150.
   Otherwise `levelHp[level]` times a multiplier of 1.0 plus 0.15 with upgrade 5 and plus 0.35 with
   upgrade 6 (`BtlCombatHasUpgrade`). It dereferences the stat table without a NULL check, so
   callers run it after `BtlCombatSetup` allocated it. */
float BtlCombatComputeMaxHp(BtlCombatState *combat)
{
    BtlBakugan *owner;
    SaveProfile *profile;
    s32 slot;
    s32 handicap;
    float hp;
    float scale;

    hp = combat->stats->levelHp[combat->level];
    owner = (BtlBakugan *)combat->owner;
    if (owner == NULL) {
        return hp;
    }
    if (g_scriptGlobalVars[8] == 2) {
        slot = owner->playerSlot;
        profile = (SaveProfile *)SaveGetProfile();
        hp = combat->stats->levelHp[4];
        handicap = 0;
        if (profile->words != NULL) {
            handicap = (s32)profile->words[0x46 + slot];
        }
        if (handicap == 50) {
            hp = hp * 0.5f;
        } else if (handicap == 150) {
            hp = hp * 1.5f;
        }
        return hp;
    }
    scale = 1.0f;
    if (BtlCombatHasUpgrade(combat, 5) != 0) {
        scale = scale + 0.150000006f;
    }
    if (BtlCombatHasUpgrade(combat, 6) != 0) {
        scale = scale + 0.349999994f;
    }
    return combat->stats->levelHp[combat->level] * scale;
}
