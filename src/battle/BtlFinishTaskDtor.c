// bdc 0x08848df8 BtlFinishTaskDtor
#include "bdc.h"

/* Destructor of the end-of-battle cinematic task `BtlFinishTask` (vtable
   `g_btlFinishTaskVtbl`, built by `BtlFinishTaskCtor`): does nothing for NULL; restores
   `g_gfxActiveCamera` from `savedCamera`, reinstalls its vtable, kills all attacks
   (`BtlAttackListKillAll`), destroys the embedded `camera` (`GfxCameraDtor` with flags 2, no
   free), runs the base `CoreTaskDestroy` and frees the task (under `MemLock`) when `flags &
   1`. */
void BtlFinishTaskDtor(BtlFinishTask *task, u32 flags)
{
    if (task != NULL) {
        task->base.vtable = g_btlFinishTaskVtbl;
        g_gfxActiveCamera = task->savedCamera;
        BtlAttackListKillAll();
        GfxCameraDtor(&task->camera.base, 2);
        CoreTaskDestroy(&task->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(task, NULL, 0);
            MemUnlock();
        }
    }
}
