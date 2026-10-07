// bdc 0x0889aaac BtlCpuUnitDraw
#include "bdc.h"

/* Draw method of the CPU-controlled battle Bakugan class (0x6d0 bytes, vtable `0x08af21b4`, built
   by `BtlCreateBakugan` for non-player modes): forwards to `BtlBakuganDraw`. */
void BtlCpuUnitDraw(void *unit, void *ctx)
{
    BtlBakuganDraw(unit, ctx);
}
