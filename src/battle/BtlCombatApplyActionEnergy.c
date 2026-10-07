// bdc 0x08888174 BtlCombatApplyActionEnergy
#include "bdc.h"

void BtlCombatApplyActionEnergy(BtlCombatState *combat, u32 action, int amount)
{
    BtlBakugan *owner;
    const VtblEntry *pred;
    bool drained = false;
    int act;
    float gain;

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
        case 2:
        case 3:
        case 5:
        case 7:
            BtlCombatDrainEnergy(BtlCombatGetActionEnergyAmount(combat, act), combat);
            drained = true;
            break;
        case 4:
        case 11:
            gain = BtlCombatGetActionEnergyAmount(combat, act);
            BtlCombatSetEnergy(BtlCombatGetEnergy(combat) + gain, combat);
            break;
        case 6:
            BtlCombatDrainEnergy((float)amount, combat);
            drained = true;
            break;
        case 8:
        case 9:
            BtlCombatDrainEnergy(BtlCombatGetActionEnergyAmount(combat, act), combat);
            combat->regenDelay = 10.0f;
            break;
        case 10:
            BtlCombatDrainEnergy(BtlCombatGetActionEnergyAmount(combat, act), combat);
            combat->regenDelay = 15.0f;
            break;
        }
    }
    if (drained) {
        combat->regenDelay = BtlCombatIsEnergyDepleted(combat) != 0 ? 10.0f : 3.0f;
    }
}
