// bdc 0x08887998 BtlCalcDamage
#include "bdc.h"

/* Damage dispatcher used by every hit receiver: hit class 1 -> `BtlCombatCalcBaseDamage` (unit
   melee), classes 2..3 -> `BtlCombatCalcAbilityDamage` (both get all five arguments), any other
   class (<= 0 or >= 4) -> `BtlCalcHazardDamage``(attackId, 0, targetAttr)` for normal hits
   (`special == 0`) and 0 for special ones. */
float BtlCalcDamage(void *attacker, s32 hitClass, s32 attackId, s32 special, s32 targetAttr)
{
    if (hitClass == 1) {
        return BtlCombatCalcBaseDamage(attacker, hitClass, attackId, special, targetAttr);
    }
    if (hitClass == 2 || hitClass == 3) {
        return BtlCombatCalcAbilityDamage(attacker, hitClass, attackId, special, targetAttr);
    }
    if (special != 0) {
        return 0.0f;
    }
    return BtlCalcHazardDamage(attackId, 0, targetAttr);
}
