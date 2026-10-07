// bdc 0x08a2a154 BtlUnitAltIsBakugan
#include "bdc.h"

/* Returns 0 for the "is a Bakugan" (BtlBakugan and its CPU subclasses) battle-unit class test
   (virtual slot 10, `+0x54`) in BtlUnitAlt's vtable. */
int BtlUnitAltIsBakugan(void *unit)
{
    (void)unit;
    return 0;
}
