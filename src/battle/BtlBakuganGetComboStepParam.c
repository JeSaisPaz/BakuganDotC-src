// bdc 0x0886b3e0 BtlBakuganGetComboStepParam
#include "bdc.h"

/* Returns the current combo step's `BtlComboStep` parameter word: `airParam` when the air
   variant is active, `param` otherwise; used by `BtlBakuganState07Update` (hit-window/parameter
   record of the step). */

u32 BtlBakuganGetComboStepParam(BtlBakugan *self)
{
  if (self->comboAirVariant != 0) {
    return self->combos[self->comboIndex][self->comboStep].airParam;
  }
  return self->combos[self->comboIndex][self->comboStep].param;
}
