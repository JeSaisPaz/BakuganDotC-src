// bdc 0x0885c124 BtlUnitAltState01Update
#include "bdc.h"

/* State-1 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 27): calls the base
   `BtlBakuganState01Update` non-virtually. */
void BtlUnitAltState01Update(void *unit)
{
    BtlBakuganState01Update((BtlBakugan *)unit);
}
