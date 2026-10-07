// bdc 0x08888018 BtlCombatGetActionEnergyAmount
#include "bdc.h"

/* Returns the energy gain or cost of energy action `action` (0..0xb) from the unit's stat table
   (`combat->stats`): 0, 1, 2, 4 and 10 read `energyAction<n>`; 3 is `energyAction3` reduced by the
   item pair 0xd (`BtlCombatApplyItemBonus` rate 0.5, mode 1); 6 is `energyAction6 * damageScale`;
   7 is `energyAction7 * damageScale` reduced by the item pair 0xb (rate 0.5, mode 1); 0xb is the
   constant 200.0; 5, 8, 9 and any other value give 0.0. Actions 0, 4 and 0xb are gains, the rest
   costs (see `BtlCombatApplyActionEnergy`). */
float BtlCombatGetActionEnergyAmount(BtlCombatState *combat, int action)
{
    switch (action) {
    case 0:
        return combat->stats->energyAction0;
    case 1:
        return combat->stats->energyAction1;
    case 2:
        return combat->stats->energyAction2;
    case 3:
        return BtlCombatApplyItemBonus(0.5f, combat->stats->energyAction3, combat, 0xd, 1);
    case 4:
        return combat->stats->energyAction4;
    case 6:
        return combat->stats->energyAction6 * combat->stats->damageScale;
    case 7:
        return BtlCombatApplyItemBonus(0.5f, combat->stats->energyAction7 * combat->stats->damageScale,
                                       combat, 0xb, 1);
    case 10:
        return combat->stats->energyAction10;
    case 11:
        return 200.0f;
    default:
        return 0.0f;
    }
}
