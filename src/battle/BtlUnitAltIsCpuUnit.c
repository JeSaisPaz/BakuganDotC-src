// bdc 0x08a2a164 BtlUnitAltIsCpuUnit
#include "bdc.h"

/* Returns 1 for the "is a CPU-controlled unit" battle-unit class test (virtual slot 13, `+0x6c`) in
   BtlUnitAlt's vtable. */
int BtlUnitAltIsCpuUnit(void *unit)
{
    (void)unit;
    return 1;
}
