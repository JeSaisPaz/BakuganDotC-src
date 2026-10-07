// bdc 0x0886b3e0 BtlBakuganGetComboStepParam
#include "bdc.h"

/* Returns the current combo step's `BtlComboStep` hit window: `airHitWindow` when the air
   variant is active, `hitWindow` otherwise (an entry of `g_btlComboHitWindows`); used by
   `BtlBakuganState07Update` to arm the step's hit window. */

BtlHitWindowDef *BtlBakuganGetComboStepParam(BtlBakugan *self)
{
  if (self->comboAirVariant != 0) {
    return self->combos[self->comboIndex][self->comboStep].airHitWindow;
  }
  return self->combos[self->comboIndex][self->comboStep].hitWindow;
}
