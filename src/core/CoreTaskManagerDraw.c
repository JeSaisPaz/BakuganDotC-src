// bdc 0x089bf5e4 CoreTaskManagerDraw
#include "bdc.h"

/* Per-frame draw pass over `g_taskList`, called by `BootMainThread` right after the
   power-state check that follows the update pass. If the list exists and is non-empty, walks the
   nodes in priority order with `iterating` set and calls the draw virtual (vtable entry 4) of
   every live task whose flag bit 1 is clear (`CoreTaskHasFlags`); a live node with a NULL task
   stops the walk. Always finishes with `CoreListPurgeRemoved`, so removals requested during
   update or draw take effect here. */
void CoreTaskManagerDraw(void)
{
    CoreListNode *node;
    CoreTask *task;
    const VtblEntry *draw;

    if (g_taskList != NULL && g_taskList->count > 0) {
        node = CoreListFirst(g_taskList);
        g_taskList->iterating = 1;
        for (; node != NULL; node = node->next) {
            if (node->removed)
                continue;
            task = node->data;
            if (task == NULL)
                break;
            if (CoreTaskHasFlags(task, 2))
                continue;
            draw = &((const VtblEntry *)task->vtable)[4];
            ((void (*)(void *))draw->fn)((u8 *)task + draw->delta);
        }
        g_taskList->iterating = 0;
    }
    CoreListPurgeRemoved(g_taskList);
}
