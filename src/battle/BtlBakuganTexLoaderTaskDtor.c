// bdc 0x08870190 BtlBakuganTexLoaderTaskDtor
#include "bdc.h"

/* Destructor of the Bakugan texture loader task `BtlBakuganTexLoaderTask` (task id 0x1e3, vtable
   `g_btlBakuganTexLoaderTaskVtbl`): does nothing for NULL; reinstalls its vtable, deletes its
   `IoLzsPackage` `package` if still set (virtual deleting destructor, vtable entry 1, flag 3)
   and clears it, runs the base `CoreTaskDestroy` and frees the task (under `MemLock`) when
   `flags & 1`. */
void BtlBakuganTexLoaderTaskDtor(BtlBakuganTexLoaderTask *task, u32 flags)
{
    IoLzsPackage *package;

    if (task != NULL) {
        package = task->package;
        task->base.vtable = g_btlBakuganTexLoaderTaskVtbl;
        if (package != NULL) {
            const VtblEntry *dtor = &((const VtblEntry *)package->base.vtable)[1];
            ((void (*)(void *, s32))dtor->fn)((u8 *)package + dtor->delta, 3);
            task->package = NULL;
        }
        CoreTaskDestroy(&task->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(task, NULL, 0);
            MemUnlock();
        }
    }
}
