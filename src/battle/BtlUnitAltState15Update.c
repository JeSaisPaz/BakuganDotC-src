// bdc 0x0885c290 BtlUnitAltState15Update
#include "bdc.h"

/* State-15 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 41): calls the base
   `BtlBakuganState15Update` non-virtually. */
void BtlUnitAltState15Update(void *unit)
{
    BtlBakuganState15Update(unit);
}
