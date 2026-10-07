// bdc 0x08877df4 BtlLastGuardedHitSuppressesImpact
#include "bdc.h"

/* Returns 1 when the last collision hit struck a guarding collider (`g_collisionLastHitGuarded`)
   and the unit recorded with that hit (`g_collisionLastHitUnit`) is non-NULL, still in the
   battle list (`BtlBakuganListFind`) and either of kind 0x15 (its `CoreObject` word `+0x08`)
   or passes `BtlBakuganHasEnergyAndGuardAgeBelow5`; else 0. `BtlAttackResolvePendingHit` then
   plays neither the hit sound nor the impact effect. `attack` is not read. */
int BtlLastGuardedHitSuppressesImpact(void *attack)
{
    BtlBakugan *unit;

    (void)attack;
    if (g_collisionLastHitGuarded == 0) {
        return 0;
    }
    unit = g_collisionLastHitUnit;
    if (unit == NULL || BtlBakuganListFind(unit) == NULL) {
        return 0;
    }
    if (unit->base.base.unk08 == 0x15) {
        return 1;
    }
    if (BtlBakuganHasEnergyAndGuardAgeBelow5(unit) != 0) {
        return 1;
    }
    return 0;
}
