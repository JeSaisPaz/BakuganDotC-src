// bdc 0x0885dae0 BtlUnitMode4Draw
#include "bdc.h"

/* Draw virtual of the mode-4 battle unit (vtable `0x08af1e1c` slot `+0x40`): just calls
   `BtlBakuganDraw``(unit, ctx)`. */
void BtlUnitMode4Draw(void *unit, void *ctx)
{
    BtlBakuganDraw(unit, ctx);
}
