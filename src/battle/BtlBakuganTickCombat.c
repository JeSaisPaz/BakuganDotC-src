// bdc 0x08862bf4 BtlBakuganTickCombat
#include "bdc.h"

/* Per-frame tick of the unit's embedded BtlCombatState (combat): energy regeneration
   (BtlCombatTickEnergyRegen with mode 0 while state is 0 or 1, otherwise mode 1), then the
   timed statuses (BtlCombatUpdateStatuses), then the special-art charge (BtlCombatUpdateArtCharge). */
void BtlBakuganTickCombat(BtlBakugan *self)
{
    s32 regenMode;
    BtlCombatState *combat;

    regenMode = 1;
    if (self->state >= 0 && self->state < 2) {
        regenMode = 0;
    }
    combat = &self->combat;
    BtlCombatTickEnergyRegen(combat, regenMode);
    BtlCombatUpdateStatuses(combat);
    BtlCombatUpdateArtCharge(combat);
}
