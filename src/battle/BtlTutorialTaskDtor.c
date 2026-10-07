// bdc 0x088466ec BtlTutorialTaskDtor
#include "bdc.h"

/* Destructor of the battle tutorial task: restores the tutorial vtable, clears script global
   variable entry 11 (g_scriptGlobalVars[11]), chains to CoreTaskDestroy without freeing, then
   frees the object when bit 0 of `flags` is set. Does nothing for NULL. */

void BtlTutorialTaskDtor(CoreTask *task, u32 flags)
{
    if (task == NULL) {
        return;
    }
    task->vtable = g_btlTutorialTaskVtbl;
    g_scriptGlobalVars[11] = 0;
    CoreTaskDestroy(task, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(task, NULL, 0);
        MemUnlock();
    }
}
