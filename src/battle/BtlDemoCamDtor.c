// bdc 0x088fdd4c BtlDemoCamDtor
#include "bdc.h"

/* Deleting destructor of the battle demo camera (slot 1 of `g_btlDemoCamVtbl`,
   `BtlDemoCamCtor`): re-installs that vtable, runs the camera base destructor
   `GfxCameraDtor` without freeing, then frees the object under `MemLock` when bit 0 of
   `flags` is set. Does nothing for NULL. */

void BtlDemoCamDtor(BtlDemoCam *self, u32 flags)
{
    if (self != NULL) {
        self->base.base.vtable = g_btlDemoCamVtbl;
        GfxCameraDtor(&self->base.base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
