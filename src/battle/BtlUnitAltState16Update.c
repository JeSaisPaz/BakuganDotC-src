// bdc 0x0885c2ac BtlUnitAltState16Update
#include "bdc.h"

/* State-16 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 42): calls the base
   `BtlBakuganState16Update` non-virtually. */
void BtlUnitAltState16Update(void *unit)
{
    BtlBakuganState16Update(unit);
}
