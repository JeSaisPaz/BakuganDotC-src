// bdc 0x0885c108 BtlUnitAltState00Update
#include "bdc.h"

/* State-0 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 26): calls the base
   `BtlBakuganState00Update` non-virtually. */
void BtlUnitAltState00Update(void *unit)
{
    BtlBakuganState00Update(unit);
}
