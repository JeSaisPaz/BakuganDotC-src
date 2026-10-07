// bdc 0x0886647c BtlBakuganSetDefenseLevel
#include "bdc.h"

/* Sets a battle unit's defense level: writes `level` unclamped to the embedded `BtlCombatState`'s
   defense level (`BtlCombatGetDefenseLevelFactor` clamps it to 0..9) and a copy clamped to 0..9
   to the unit's own `defenseLevel`. */
void BtlBakuganSetDefenseLevel(BtlBakugan *bakugan, s32 level)
{
    if (level < 0) {
        bakugan->defenseLevel = 0;
    } else if (level > 9) {
        bakugan->defenseLevel = 9;
    } else {
        bakugan->defenseLevel = level;
    }
    bakugan->combat.defenseLevel = level;
}
