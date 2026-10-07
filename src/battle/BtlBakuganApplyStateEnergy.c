// bdc 0x0885fee4 BtlBakuganApplyStateEnergy
#include "bdc.h"

/* Applies the energy effect of an action state to the unit's `BtlCombatState` (`combat`): when
   `inWater` is set, the unit's vtable slot 20 does not return 1 and the unit kind is not 0xf, uses
   `BtlCombatApplyActionEnergyInWater`; otherwise `BtlCombatApplyActionEnergy`, passing `amount`
   only for state 6 (getting hit). */
void BtlBakuganApplyStateEnergy(BtlBakugan *self, u32 state, int amount)
{
    bool inWater = false;
    BtlCombatState *combat = &self->combat;
    const VtblEntry *vtbl;

    if (self->inWater != 0) {
        vtbl = (const VtblEntry *)self->base.base.vtable;
        if (((int (*)(void *))vtbl[20].fn)((u8 *)self + vtbl[20].delta) != 1) {
            inWater = true;
        }
    }
    if (inWater && self->base.base.unk08 != 0xf) {
        BtlCombatApplyActionEnergyInWater(combat, state);
    } else if (state == 6) {
        BtlCombatApplyActionEnergy(combat, 6, amount);
    } else {
        BtlCombatApplyActionEnergy(combat, state, 0);
    }
}
