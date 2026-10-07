// bdc 0x088710c8 BtlBakuganIsCounterWindowOpen
#include "bdc.h"

/* Returns 1 when the unit can counter right now: it has a recorded `attacker`, no
   `lockedAttacker`, the attacker's flag 0x200 is set, the unit is in state 3, has energy
   (`BtlCombatHasEnergy`), its `counterWindow` timer is positive and the attacker's combat status
   10 is not active; else 0. Used by `BtlAiUpdate` for the CPU counter decision. */
int BtlBakuganIsCounterWindowOpen(BtlBakugan *self)
{
    BtlBakugan *attacker = (BtlBakugan *)self->attacker;

    if (attacker == NULL || self->lockedAttacker != NULL) {
        return 0;
    }
    if ((attacker->flags & 0x200) == 0 || self->state != 3) {
        return 0;
    }
    if (BtlCombatHasEnergy(&self->combat) == 0 || self->counterWindow <= 0) {
        return 0;
    }
    if (((BtlBakugan *)self->attacker)->combat.status[10].active != 0) {
        return 0;
    }
    return 1;
}
