// bdc 0x0885c178 BtlUnitAltState04Update
#include "bdc.h"

/* State-4 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 30): calls the base
   `BtlBakuganState04Update` non-virtually. */

void BtlUnitAltState04Update(void *unit)
{
    BtlBakuganState04Update((BtlBakugan *)unit);
}
