// bdc 0x08a2a15c BtlUnitAltIsUnitAlt
#include "bdc.h"

/* Returns 1 for the "is the non-playable BtlUnitAlt" battle-unit class test (virtual slot 12,
   `+0x64`) in BtlUnitAlt's vtable. */
int BtlUnitAltIsUnitAlt(void *unit)
{
    (void)unit;
    return 1;
}
