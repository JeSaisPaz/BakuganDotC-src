// bdc 0x08a29ed0 BtlBakuganIsUnitAlt
#include "bdc.h"

/* Returns 0 for the "is the non-playable BtlUnitAlt" battle-unit class test (virtual slot 12,
   `+0x64`) in BtlBakugan's vtables. */
int BtlBakuganIsUnitAlt(void *unit)
{
    (void)unit;
    return 0;
}
