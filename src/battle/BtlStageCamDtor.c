// bdc 0x089039ec BtlStageCamDtor
#include "bdc.h"

/* Destructor of the stage camera demo task (task id 0x6b, 0x58 bytes, slot 1 of
   `g_btlStageCamVtbl`, `BtlStageCamCtor`): for a non-NULL task, reinstalls
   `g_btlStageCamVtbl`; frees the camera scene `scene` (`BtlStageCamSceneUnload`, then
   `BtlStageCamSceneDtor` with flags 3 and clears the pointer), deletes the demo camera `cam`
   through its virtual deleting destructor (vtable slot 1, flags 3) and clears it, then runs
   `CoreTaskDestroy``(self, 0)`; frees the task (`MemFree` under `MemLock`) when bit 0 of
   `flags` is set. */
void BtlStageCamDtor(BtlStageCam *self, u32 flags)
{
    if (self == NULL) {
        return;
    }
    self->base.vtable = g_btlStageCamVtbl;
    if (self->scene != NULL) {
        BtlStageCamSceneUnload(self->scene);
        if (self->scene != NULL) {
            BtlStageCamSceneDtor(self->scene, 3);
            self->scene = NULL;
        }
    }
    if (self->cam != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)self->cam->base.base.vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)self->cam + dtor->delta, 3);
        self->cam = NULL;
    }
    CoreTaskDestroy(&self->base, 0);
    if (flags & 1) {
        MemLock();
        MemFree(self, NULL, 0);
        MemUnlock();
    }
}
