// bdc 0x089bf6a8 CoreTaskCreate
#include "bdc.h"

/* Creates a task by id and registers it: asks the task factory `CoreTaskNewById` for a new task;
   on success inserts it into `g_taskList` with `priority` (`id == 100` forces priority 200) via
   `CoreListInsert` and stores `id` in the task. Returns the task, or NULL when the factory does
   not know the id or allocation failed. */
CoreTask *CoreTaskCreate(s32 id, s32 priority)
{
    CoreTask *task;

    task = CoreTaskNewById(id);
    if (task != NULL) {
        if (g_taskList != NULL) {
            if (id == 100) {
                priority = 200;
            }
            CoreListInsert(g_taskList, task, priority);
        }
        task->id = id;
    }
    return task;
}
