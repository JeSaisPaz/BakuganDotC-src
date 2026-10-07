// bdc 0x088603a0 BtlBakuganHasEnergyAndGuardAgeBelow5
#include "bdc.h"

/* Returns 1 when the unit's `BtlCombatState` has energy (`BtlCombatHasEnergy`) and its
   signed guard age `guardAge` (`+0x398`, the counter reset by `BtlBakuganStartGuardEffect`) is
   below 5, else 0. Used by `BtlLastGuardedHitSuppressesImpact`. */
int BtlBakuganHasEnergyAndGuardAgeBelow5(BtlBakugan *bakugan)
{
    if (BtlCombatHasEnergy(&bakugan->combat) != 0 && bakugan->guardAge < 5) {
        return 1;
    }
    return 0;
}
