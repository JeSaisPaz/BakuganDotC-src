// bdc 0x0885c554 BtlUnitAltState21Update
#include "bdc.h"

/* State-21 handler of `BtlUnitAlt` (vtable `0x08af1c94` slot 47): calls the base
   `BtlBakuganState21Update` non-virtually. */
void BtlUnitAltState21Update(void *unit)
{
    BtlBakuganState21Update(unit);
}
