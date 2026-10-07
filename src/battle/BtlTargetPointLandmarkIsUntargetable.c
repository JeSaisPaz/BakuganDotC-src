// bdc 0x08a2a0d0 BtlTargetPointLandmarkIsUntargetable
#include "bdc.h"

/* Landmark target-point override of `BtlBakuganIsUntargetable` (entry 17, fn at `+0x8c`): returns
   the untargetable flag byte, zero-extended. */
int BtlTargetPointLandmarkIsUntargetable(BtlTargetPointLandmark *unit)
{
    return unit->untargetable;
}
