// bdc 0x08847188 BtlCameraDtor
#include "bdc.h"

/* Destructor of the battle camera controller: does nothing for NULL; otherwise restores
   g_btlCameraVtbl, runs the base camera destructor (GfxCameraDtor, flags 0) and frees the object
   when bit 0 of `flags` is set. */
void BtlCameraDtor(BtlCamera *camera, u32 flags)
{
    if (camera == NULL) {
        return;
    }
    camera->base.base.vtable = g_btlCameraVtbl;
    GfxCameraDtor(&camera->base.base, 0);
    if (flags & 1) {
        MemLock();
        MemFree(camera, NULL, 0);
        MemUnlock();
    }
}
