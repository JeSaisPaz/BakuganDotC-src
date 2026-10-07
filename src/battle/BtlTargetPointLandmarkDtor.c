// bdc 0x08a2a0e0 BtlTargetPointLandmarkDtor
#include "bdc.h"

/* Destructor of the landmark TargetPoint unit (slot 1 of `g_btlTargetPointLandmarkVtbl`, class
   built by `BtlTargetPointLandmarkCtor`): for a non-NULL unit, reinstalls
   `g_btlTargetPointLandmarkVtbl`, runs the parent destructor `BtlTargetPointDtor``(self, 0)`
   (which chains to `BtlBakuganDtor`) and frees the object (`MemFree` under `MemLock`) when
   bit 0 of `flags` is set. */
void BtlTargetPointLandmarkDtor(BtlTargetPointLandmark *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->base.base.base.vtable = g_btlTargetPointLandmarkVtbl;
    BtlTargetPointDtor(&self->base, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
