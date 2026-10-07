// bdc 0x08863118 BtlBakuganSetHpThreshold
#include "bdc.h"

/* Battle-unit virtual (entry 21, fn at vtable `+0xac`): stores `threshold` (an HP ratio) in
   `hpThreshold`, the value tested by `BtlBakuganIsHpAtOrBelowThreshold`. */

void BtlBakuganSetHpThreshold(BtlBakugan *bakugan, float threshold)
{
    bakugan->hpThreshold = threshold;
}
