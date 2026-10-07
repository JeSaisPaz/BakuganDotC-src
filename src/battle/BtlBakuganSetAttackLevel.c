// bdc 0x0886643c BtlBakuganSetAttackLevel
#include "bdc.h"

/* Sets a battle unit's attack level: writes `level` unclamped to the embedded `BtlCombatState`'s
   attack level (which `BtlCombatGetAttackLevelFactor` clamps to 0..9 and maps to 0.70..1.25) and
   a copy clamped to 0..9 to the unit's own `attackLevel`. */
void BtlBakuganSetAttackLevel(BtlBakugan *bakugan, s32 level)
{
    s32 clamped = level;

    if (level < 0) {
        clamped = 0;
    } else if (level > 9) {
        clamped = 9;
    }
    bakugan->attackLevel = clamped;
    bakugan->combat.attackLevel = level;
}
