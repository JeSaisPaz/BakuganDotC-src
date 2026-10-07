// bdc 0x0885c1b0 BtlUnitAltState06Update
#include "bdc.h"

/* State-6 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 32): calls the base
   `BtlBakuganState06Update` non-virtually. */
void BtlUnitAltState06Update(void *unit)
{
    BtlBakuganState06Update(unit);
}
