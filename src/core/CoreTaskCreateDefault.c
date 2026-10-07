// bdc 0x089bf714 CoreTaskCreateDefault
#include "bdc.h"

/* Variant of `CoreTaskCreate` that builds the task through the second task factory
   (`CoreTaskNewByIdArg(id, arg)`, `arg` forwarded untouched: the unit for the cut-in task, a small
   integer for the others) and always inserts it into `g_taskList` with priority 100
   (`CoreListInsert`, the list insert behind `CoreTaskInsert`); stores `id` in the task and
   returns it, or NULL when the factory has no class for `id`. About ten callers, among them
   `ScriptOpCreateTask`. */
CoreTask *CoreTaskCreateDefault(s32 id, void *arg)
{
    CoreTask *task;

    task = CoreTaskNewByIdArg(id, arg);
    if (task != NULL) {
        if (g_taskList != NULL) {
            CoreListInsert(g_taskList, task, 100);
        }
        task->id = id;
    }
    return task;
}
