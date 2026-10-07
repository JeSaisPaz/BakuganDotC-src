// bdc 0x0885c204 BtlUnitAltState09Update
#include "bdc.h"

/* State-9 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 35): calls the base
   `BtlBakuganState09Update` non-virtually. */
void BtlUnitAltState09Update(void *unit)
{
    BtlBakuganState09Update(unit);
}
