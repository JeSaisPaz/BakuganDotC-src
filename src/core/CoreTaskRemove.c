// bdc 0x089bf768 CoreTaskRemove
#include "bdc.h"

/* Removes `task` from `g_taskList` and, when `destroy` is set and the removal succeeded, runs
   its virtual deleting destructor (vtable entry 1) with flag 3 (destruct and delete). Before that
   it clears the exclusive task id (`CoreTaskSetExclusiveId``(0)`) if it equals the task's id.
   Does nothing without a task list. Used all over the game to kill tasks (30+ callers), by
   `CoreTaskManagerDestroy` for every remaining task and by `ScriptOpControlMovie` /
   `ScriptOpPlayMovie`. */
void CoreTaskRemove(CoreTask *task, bool destroy)
{
    s32 id;
    const VtblEntry *dtor;

    if (g_taskList == NULL)
        return;
    id = task->id;
    if (CoreTaskGetExclusiveId() == (u32)id)
        CoreTaskSetExclusiveId(0);
    if (CoreListRemove(g_taskList, task) && destroy && task != NULL) {
        dtor = &((const VtblEntry *)task->vtable)[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)task + dtor->delta, 3);
    }
}
