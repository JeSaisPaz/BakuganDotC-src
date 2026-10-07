// bdc 0x0885c258 BtlUnitAltState13Update
#include "bdc.h"

/* State-13 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 39): calls the base
   `BtlBakuganState13Update` non-virtually. */

void BtlUnitAltState13Update(void *unit)
{
    BtlBakuganState13Update(unit);
}
