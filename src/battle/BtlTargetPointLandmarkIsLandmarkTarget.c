// bdc 0x08a2a0d8 BtlTargetPointLandmarkIsLandmarkTarget
#include "bdc.h"

/* Landmark target-point override of `BtlBakuganIsLandmarkTarget` (entry 19, fn at `+0x9c`):
   returns 1. */

int BtlTargetPointLandmarkIsLandmarkTarget(void *unit)
{
    (void)unit;
    return 1;
}
