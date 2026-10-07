// bdc 0x08887b6c BtlCombatApplyDamage
#include "bdc.h"

/* Subtracts `damage` from a unit's hit points. Does nothing and returns 0.0 when the unit is
   already dead or `damage <= 0`. Otherwise computes `hp - damage` (`BtlCombatGetHp`); with
   `nonLethal` set a result `<= 0` becomes 1.0, and a result still `<= 0` sets `dead` and becomes 0;
   stores it with `BtlCombatSetHp`. When the battle rule mode (script global var 8) is 1 and the
   owner's vtable entry 22 (`BtlBakuganIsHpAtOrBelowThreshold` for the base class) returns
   non-zero, HP is reset to `maxHp * hpThreshold` (max HP converted as unsigned) and `dead` cleared.
   Returns `damage`. `attacker` is unused. */
float BtlCombatApplyDamage(float damage, BtlCombatState *combat, int attacker, u8 nonLethal)
{
    float hp;

    (void)attacker;
    if (combat->dead || damage <= 0.0f) {
        return 0.0f;
    }
    hp = BtlCombatGetHp(combat) - damage;
    if (nonLethal && hp <= 0.0f) {
        hp = 1.0f;
    }
    if (hp <= 0.0f) {
        combat->dead = 1;
        hp = 0.0f;
    }
    BtlCombatSetHp(hp, combat);
    if (g_scriptGlobalVars[8] == 1) {
        BtlBakugan *owner = (BtlBakugan *)combat->owner;
        const VtblEntry *atThreshold = &((const VtblEntry *)owner->base.base.vtable)[22];

        if (((int (*)(void *))atThreshold->fn)((u8 *)owner + atThreshold->delta) != 0) {
            float threshold = owner->hpThreshold;

            BtlCombatSetHp((float)(u32)BtlCombatGetMaxHp(combat) * threshold, combat);
            combat->dead = 0;
        }
    }
    return damage;
}
