// bdc 0x08888404 BtlCombatApplyActionEnergyInWater
#include "bdc.h"

/* In-water version of `BtlCombatApplyActionEnergy`: the same energy-action switch (jump table
   `0x08a68e40`), but the costs of actions 1, 3, 6, 7 and 10 are multiplied by 1.5, or by 2.25 when
   the unit's attribute (stat-table byte 0) is 0. Action 6 drains the table amount
   (`BtlCombatGetActionEnergyAmount`) here, since there is no `amount` parameter. Gains (0, 4,
   0xb), the regeneration delay `combat+0xa4` (10.0 for 8/9, 15.0 for 10, 3.0/10.0 after the other
   costs via `BtlCombatIsEnergyDepleted`) and the airborne check for action 0 are unchanged. */

void BtlCombatApplyActionEnergyInWater(BtlCombatState *combat, u32 action)
{
    BtlBakugan *owner;
    const VtblEntry *pred;
    bool drained = false;
    float scale = 1.5f;
    float gain;
    int act;

    if (combat->stats->attribute == 0) {
        scale = scale * scale;
    }
    if (action < 12) {
        act = (int)action;
        switch (action) {
        case 0:
            owner = combat->owner;
            pred = &((const VtblEntry *)owner->base.base.vtable)[10];
            if (((int (*)(void *))pred->fn)((u8 *)owner + pred->delta) != 0) {
                if (BtlBakuganIsAirborne(combat->owner, 1) != 0) {
                    break;
                }
            }
            gain = BtlCombatGetActionEnergyAmount(combat, act);
            BtlCombatSetEnergy(BtlCombatGetEnergy(combat) + gain, combat);
            break;
        case 1:
        case 3:
        case 6:
        case 7:
            BtlCombatDrainEnergy(BtlCombatGetActionEnergyAmount(combat, act) * scale, combat);
            drained = true;
            break;
        case 2:
        case 5:
            BtlCombatDrainEnergy(BtlCombatGetActionEnergyAmount(combat, act), combat);
            drained = true;
            break;
        case 4:
        case 11:
            gain = BtlCombatGetActionEnergyAmount(combat, act);
            BtlCombatSetEnergy(BtlCombatGetEnergy(combat) + gain, combat);
            break;
        case 8:
        case 9:
            BtlCombatDrainEnergy(BtlCombatGetActionEnergyAmount(combat, act), combat);
            combat->regenDelay = 10.0f;
            break;
        case 10:
            BtlCombatDrainEnergy(BtlCombatGetActionEnergyAmount(combat, act) * scale, combat);
            combat->regenDelay = 15.0f;
            break;
        }
    }
    if (drained) {
        combat->regenDelay = BtlCombatIsEnergyDepleted(combat) ? 10.0f : 3.0f;
    }
}
