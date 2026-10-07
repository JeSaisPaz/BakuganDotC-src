// bdc 0x0885c140 BtlUnitAltState02Update
#include "bdc.h"

/* State-2 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 28): calls the base
   `BtlBakuganState02Update` non-virtually. */
void BtlUnitAltState02Update(void *unit)
{
    BtlBakuganState02Update(unit);
}
