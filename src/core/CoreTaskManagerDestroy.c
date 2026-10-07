// bdc 0x089bf3fc CoreTaskManagerDestroy
#include "bdc.h"

/* Destructor of the task manager: if `this` exists, removes every task still in `g_taskList`
   (`CoreTaskRemove``(task, true)` per node), destroys the list itself (`CoreListDestroy``(list,
   3)`) and clears `g_taskList`, tears down the helpers created by `CoreTaskManagerInit`
   (`GfxDestroyNullTexture`, `SndBgmCmdListDestroyGlobal`) and, when bit 0 of `flags` is set,
   frees `this` with `MemFree` under `MemLock`. */
void CoreTaskManagerDestroy(void *this, u32 flags)
{
    if (this == NULL) {
        return;
    }
    if (g_taskList != NULL && g_taskList->count > 0) {
        CoreListNode *node;

        for (node = CoreListFirst(g_taskList); node != NULL; node = node->next) {
            CoreTaskRemove((CoreTask *)node->data, true);
        }
    }
    if (g_taskList != NULL) {
        CoreListDestroy(g_taskList, 3);
        g_taskList = NULL;
    }
    GfxDestroyNullTexture();
    SndBgmCmdListDestroyGlobal();
    if (flags & 1) {
        MemLock();
        MemFree(this, NULL, 0);
        MemUnlock();
    }
}
