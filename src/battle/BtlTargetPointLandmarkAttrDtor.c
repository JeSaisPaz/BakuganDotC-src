// bdc 0x08a2a05c BtlTargetPointLandmarkAttrDtor
#include "bdc.h"

/* Destructor (slot 1 of `g_btlTargetPointLandmarkAttrVtbl`) of the class built by
   `BtlTargetPointLandmarkAttrCtor`: for a non-NULL object, reinstalls
   `g_btlTargetPointLandmarkAttrVtbl`, runs the parent destructor `BtlTargetPointDtor``(self, 0)`
   (which restores `g_btlTargetPointVtbl` and chains to `BtlBakuganDtor`) and frees the object
   (`MemFree` under `MemLock`) when bit 0 of `flags` is set. */

void BtlTargetPointLandmarkAttrDtor(BtlBakugan *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->base.base.vtable = g_btlTargetPointLandmarkAttrVtbl;
    BtlTargetPointDtor(self, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
