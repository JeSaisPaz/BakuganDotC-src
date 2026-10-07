// bdc 0x08863120 BtlBakuganIsHpAtOrBelowThreshold
#include "bdc.h"

/* Battle-unit virtual (vtable `0x08af1fa4` slot `+0xb0`): returns 1 when the HP threshold
   `hpThreshold` is not <= 0 and the HP ratio (`BtlCombatGetHpRatio`) is at or below it, else 0. */
int BtlBakuganIsHpAtOrBelowThreshold(BtlBakugan *self)
{
  if (!(self->hpThreshold <= 0.0f) && BtlCombatGetHpRatio(&self->combat) <= self->hpThreshold) {
    return 1;
  }
  return 0;
}
