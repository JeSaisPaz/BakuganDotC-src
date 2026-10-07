// bdc 0x08865dfc BtlBakuganValidateComboLock
#include "bdc.h"

/* Revalidates the unit's locked attacker: keeps it only while it is still in the battle list
   (`BtlBakuganListFind`) and in state 0xb (combo/ability, `BtlBakuganState11Update`);
   otherwise clears it. Returns whether a lock remains. Called by `BtlBakuganUpdateTargeting`. */
int BtlBakuganValidateComboLock(BtlBakugan *self)
{
    BtlBakugan *locked = BtlBakuganListFind(self->lockedAttacker);

    self->lockedAttacker = locked;
    if (locked != NULL && locked->state != 0xb) {
        self->lockedAttacker = NULL;
        locked = NULL;
    }
    return locked != NULL;
}
