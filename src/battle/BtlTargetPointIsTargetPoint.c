// bdc 0x08a29fd0 BtlTargetPointIsTargetPoint
#include "bdc.h"

/* Target-point override of the class predicate at vtable entry 14: always returns 1 (the base
   implementation returns 0). */
int BtlTargetPointIsTargetPoint(void *unit)
{
    (void)unit;
    return 1;
}
