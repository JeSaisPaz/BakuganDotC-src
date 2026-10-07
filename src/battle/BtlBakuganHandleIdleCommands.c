// bdc 0x08871754 BtlBakuganHandleIdleCommands
#include "bdc.h"

/* Shared command dispatch of the idle-like states (0, 3, 19), checked in this order: attack
   command → `BtlBakuganStartAttackOrArt`; command bit 1 → state 1; command bit 2 with energy
   (`BtlCombatHasEnergy`) → state 0xc; command bit 4 with energy → state 2 (dash); otherwise,
   unless `noFall`, state 0x17 when `BtlBakuganIsAirborne``(unit, 1)`. Returns 1 when a state
   change was requested, else 0. */
int BtlBakuganHandleIdleCommands(BtlBakugan *self, char noFall)
{
    if (BtlBakuganHasAttackCommand(self)) {
        BtlBakuganStartAttackOrArt(self, 0);
        return 1;
    }
    if (self->commands & 1) {
        BtlBakuganSetState(self, 1, 0);
        return 1;
    }
    if ((self->commands & 2) && BtlCombatHasEnergy(&self->combat)) {
        BtlBakuganSetState(self, 0xc, 0);
        return 1;
    }
    if ((self->commands & 4) && BtlCombatHasEnergy(&self->combat)) {
        BtlBakuganSetState(self, 2, 0);
        return 1;
    }
    if (!noFall && BtlBakuganIsAirborne(self, 1)) {
        BtlBakuganSetState(self, 0x17, 0);
        return 1;
    }
    return 0;
}
