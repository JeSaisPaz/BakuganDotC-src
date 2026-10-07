// bdc 0x089bf888 CoreTaskRemoveAllById
#include "bdc.h"

/* Removes every task with id `id` from `g_taskList` (`CoreTaskRemove``(task, true)` on each
   match); `CoreTaskRemoveById` stops at the first. The walk stops at the first node without
   data; the next link is read after the removal. */
void CoreTaskRemoveAllById(s32 id)
{
    CoreListNode *node;
    CoreTask *task;

    if (g_taskList == NULL) {
        return;
    }
    for (node = CoreListFirst(g_taskList); node != NULL; node = node->next) {
        task = node->data;
        if (task == NULL) {
            break;
        }
        if (task->id == id) {
            CoreTaskRemove(task, true);
        }
    }
}
