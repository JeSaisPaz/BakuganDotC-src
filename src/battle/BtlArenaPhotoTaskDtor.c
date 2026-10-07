// bdc 0x08845d00 BtlArenaPhotoTaskDtor
#include "bdc.h"

/* Destructor (`g_btlArenaPhotoTaskVtbl` entry 1) of the arena portrait loader: reinstalls the
   vtable, waits for the GE (`GfxWaitGeIdle`), then for each of the 3 slots releases the pending
   load (`IoDataMngRelease` with the task as owner), frees the path string and deletes the
   portrait object through its vtable (entry 1, flags 3), clearing each; chains to
   `CoreTaskDestroy``(task, 0)` and frees the task when `flags & 1`. Does nothing for NULL. */
void BtlArenaPhotoTaskDtor(BtlArenaPhotoTask *task, u32 flags)
{
    int i;

    if (task == NULL) {
        return;
    }
    task->base.vtable = g_btlArenaPhotoTaskVtbl;
    GfxWaitGeIdle();
    for (i = 0; i < 3; i++) {
        CoreObject *sprite;

        if (task->loads[i] != NULL) {
            IoDataMngRelease((IoDataMng *)IoGetDataMng(), task, task->loads[i]);
            task->loads[i] = NULL;
        }
        if (task->paths[i] != NULL) {
            MemLock();
            MemFree(task->paths[i], NULL, 0);
            MemUnlock();
            task->paths[i] = NULL;
        }
        sprite = task->sprites[i];
        if (sprite != NULL) {
            const VtblEntry *dtor = &((const VtblEntry *)sprite->vtable)[1];

            ((void (*)(void *, s32))dtor->fn)((u8 *)sprite + dtor->delta, 3);
            task->sprites[i] = NULL;
        }
    }
    CoreTaskDestroy(&task->base, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(task, NULL, 0);
        MemUnlock();
    }
}
