// bdc 0x0885bd58 BtlUnitAltDraw
#include "bdc.h"

/* Slot 8 (draw) of BtlUnitAlt: forwards `unit` and the draw context `ctx` unchanged
   to BtlBakuganDraw. */
void BtlUnitAltDraw(void *unit, void *ctx)
{
    BtlBakuganDraw(unit, ctx);
}
