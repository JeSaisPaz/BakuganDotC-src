// bdc 0x08a2a17c BtlUnitMode4IsMode4Unit
#include "bdc.h"

/* Mode-4 unit override of `BtlBakuganIsMode4Unit` (entry 18, fn at `+0x94`): returns 1. */
int BtlUnitMode4IsMode4Unit(void *unit)
{
    (void)unit;
    return 1;
}
