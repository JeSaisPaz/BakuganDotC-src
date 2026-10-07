// bdc 0x08a29fe0 BtlTargetPointPropDtor
#include "bdc.h"

/* Destructor of the TargetPoint prop unit (slot 1 of `g_btlTargetPointPropVtbl`, object built by
   `BtlTargetPointPropCtor`): for a non-NULL unit, restores `g_btlTargetPointPropVtbl`, runs the
   parent destructor `BtlTargetPointDtor``(self, 0)` and frees the object (`MemFree` under
   `MemLock`) when bit 0 of `flags` is set. */

void BtlTargetPointPropDtor(BtlBakugan *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->base.base.vtable = g_btlTargetPointPropVtbl;
    BtlTargetPointDtor(self, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
