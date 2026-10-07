// bdc 0x0885c194 BtlUnitAltState05Update
#include "bdc.h"

/* State-5 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 31): calls the base
   `BtlBakuganState05Update` non-virtually. */
void BtlUnitAltState05Update(void *unit)
{
    BtlBakuganState05Update(unit);
}
