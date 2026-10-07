// bdc 0x088547ac BtlCutInTaskDtor
#include "bdc.h"

/* Destructor of the battle cut-in task (`BtlCutInTask`, slot 1 of `g_btlCutInTaskVtbl`): for a
   non-NULL task, restores `g_btlCutInTaskVtbl`, waits for the GE (`GfxWaitGeIdle`), deletes the
   effect manager `effects` through its vtable slot 1 with flags 3 and clears the pointer, sets
   `g_btlCameraDefaultMode` to 1, runs `CoreTaskDestroy``(task, 0)` and frees the task
   (`MemFree` under `MemLock`) when bit 0 of `flags` is set. */

void BtlCutInTaskDtor(BtlCutInTask *self, u32 flags)
{
    const VtblEntry *dtor;

    if (self == NULL) {
        return;
    }
    self->base.vtable = g_btlCutInTaskVtbl;
    GfxWaitGeIdle();
    if (self->effects != NULL) {
        dtor = &self->effects->base.vtbl[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)self->effects + dtor->delta, 3);
        self->effects = NULL;
    }
    g_btlCameraDefaultMode = 1;
    CoreTaskDestroy(&self->base, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
