// bdc 0x089bf810 CoreTaskRemoveById
#include "bdc.h"

/* Destroys the first task in `g_taskList` whose id equals `id`: walks the list and calls
   `CoreTaskRemove``(task, 1)` on the first match (unlinks it and runs its deleting destructor).
   Does nothing when the list is missing or no task matches; the walk stops at the first node
   without data. */
void CoreTaskRemoveById(s32 id)
{
    CoreListNode *node;
    CoreTask *task;

    if (g_taskList == NULL) {
        return;
    }
    for (node = CoreListFirst(g_taskList); node != NULL; node = node->next) {
        task = node->data;
        if (task == NULL) {
            return;
        }
        if (task->id == id) {
            CoreTaskRemove(task, true);
            return;
        }
    }
}
