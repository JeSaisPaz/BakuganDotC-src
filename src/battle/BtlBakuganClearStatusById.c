// bdc 0x0885fda0 BtlBakuganClearStatusById
#include "bdc.h"

/* Clears the combat status behind status-visual id `which` on the unit's `BtlCombatState`
   (`BtlCombatClearStatus`): 1 zeroes `statusTimer`, 2 clears status 0x12, 3 status 0x13, 4
   status 1, 5 status 3, 6 status 5, 7 statuses 1, 3 and 5; 0 clears 0x12, 0x13, 1, 3 and 5 and
   then zeroes `statusTimer`. Other values do nothing. */
void BtlBakuganClearStatusById(BtlBakugan *self, int which)
{
    if (which == 0) {
        BtlCombatState *combat = &self->combat;

        BtlCombatClearStatus(combat, 0x12);
        BtlCombatClearStatus(combat, 0x13);
        BtlCombatClearStatus(combat, 1);
        BtlCombatClearStatus(combat, 3);
        BtlCombatClearStatus(combat, 5);
        self->statusTimer = 0;
        return;
    }
    if (which == 1) {
        self->statusTimer = 0;
    }
    if (which == 2) {
        BtlCombatClearStatus(&self->combat, 0x12);
    }
    if (which == 3) {
        BtlCombatClearStatus(&self->combat, 0x13);
    }
    if (which == 4 || which == 7) {
        BtlCombatClearStatus(&self->combat, 1);
    }
    if (which == 5 || which == 7) {
        BtlCombatClearStatus(&self->combat, 3);
    }
    if (which == 6 || which == 7) {
        BtlCombatClearStatus(&self->combat, 5);
    }
}
