// bdc 0x0885c1e8 BtlUnitAltState08Update
#include "bdc.h"

/* State-8 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 34): calls the base
   `BtlBakuganState08Update` non-virtually. */
void BtlUnitAltState08Update(void *unit)
{
    BtlBakuganState08Update(unit);
}
