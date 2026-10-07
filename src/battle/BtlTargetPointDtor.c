// bdc 0x0885b984 BtlTargetPointDtor
#include "bdc.h"

/* Destructor of the TargetPoint dummy unit (slot 1 of `g_btlTargetPointVtbl`, also chained from
   its subclasses' destructors): for a non-NULL unit, restores `g_btlTargetPointVtbl`, runs
   `BtlBakuganDtor``(self, 0)` and frees the object (`MemFree` under `MemLock`) when bit 0 of
   `flags` is set. */

void BtlTargetPointDtor(BtlBakugan *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->base.base.vtable = g_btlTargetPointVtbl;
    BtlBakuganDtor(self, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
