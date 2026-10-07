// bdc 0x0885c15c BtlUnitAltState03Update
#include "bdc.h"

/* State-3 handler of `BtlUnitAlt` (vtable `0x08af1c94` entry 29, `+0xe8`): calls
   the base `BtlBakuganState03Update` non-virtually. */
void BtlUnitAltState03Update(void *unit)
{
    BtlBakuganState03Update(unit);
}
