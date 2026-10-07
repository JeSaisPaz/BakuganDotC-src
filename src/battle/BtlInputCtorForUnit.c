// bdc 0x08884808 BtlInputCtorForUnit
#include "bdc.h"

/* Constructs the input controller of a battle unit: installs `g_btlInputVtbl`, stores the owner
   `unit`, selects mode 0 (unit fields), then `BtlInputReset`. Called by `BtlBakuganCtor`; the
   field-actor variant is `BtlInputCtorForActor`. */
void BtlInputCtorForUnit(BtlInput *self, void *unit)
{
    self->vtbl = g_btlInputVtbl;
    self->owner = unit;
    self->mode = 0;
    BtlInputReset(self);
}
