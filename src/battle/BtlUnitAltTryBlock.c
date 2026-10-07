// bdc 0x0885c714 BtlUnitAltTryBlock
#include "bdc.h"

/* Slot 25 (block) of `BtlUnitAlt`: a kind-0x15 unit uses
   `BtlUnitAltTryBlockKind15`, any other kind the base `BtlBakuganTryBlock`; returns the
   callee's result (non-zero when the hit was blocked). */
int BtlUnitAltTryBlock(BtlBakugan *unit)
{
    if (unit->base.base.unk08 == 0x15) {
        return BtlUnitAltTryBlockKind15(unit);
    }
    return BtlBakuganTryBlock(unit);
}
