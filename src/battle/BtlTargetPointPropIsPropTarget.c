// bdc 0x08a29fd8 BtlTargetPointPropIsPropTarget
#include "bdc.h"

/* Prop target-point override of the class predicate at vtable entry 15: always returns 1. */
int BtlTargetPointPropIsPropTarget(void *unit)
{
    (void)unit;
    return 1;
}
