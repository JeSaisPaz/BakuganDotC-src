// bdc 0x0885c220 BtlUnitAltState10Update
#include "bdc.h"

/* State-10 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 36, offset 0x120):
   calls the base `BtlBakuganState10Update` non-virtually on `unit`. */
void BtlUnitAltState10Update(void *unit)
{
    BtlBakuganState10Update(unit);
}
