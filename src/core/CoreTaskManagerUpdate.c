// bdc 0x089bf4d8 CoreTaskManagerUpdate
#include "bdc.h"

/* Per-frame update pass over `g_taskList`, called by `BootMainThread` with `g_taskManager`
   after the net-play pre-update. Skipped while the power manager reports the game as suspended
   (`CorePowerIsInitialized` / `CorePowerCanRunFrame`) or when the list is missing/empty.
   Otherwise it runs three housekeeping hooks (`UiTextClearAll`, `CoreObjectDeferDeleteFlush`,
   `GfxDeferredDeleteFlush`), then walks the nodes in priority order with `iterating` set and,
   for each node not flagged removed whose task `CoreTaskIsIdAllowed` accepts and whose flag bit 0
   is clear (`CoreTaskHasFlags`), calls its update virtual (vtable entry 2). */
void CoreTaskManagerUpdate(void)
{
    bool suspended;
    CoreListNode *node;
    CoreTask *task;
    const VtblEntry *update;

    suspended = false;
    if (CorePowerIsInitialized() != 0 && CorePowerCanRunFrame(CorePowerGet()) == 0) {
        suspended = true;
    }
    if (suspended || g_taskList == NULL || g_taskList->count <= 0) {
        return;
    }
    UiTextClearAll();
    CoreObjectDeferDeleteFlush();
    GfxDeferredDeleteFlush();
    node = CoreListFirst(g_taskList);
    g_taskList->iterating = 1;
    for (; node != NULL; node = node->next) {
        if (node->removed != 0) {
            continue;
        }
        task = node->data;
        if (CoreTaskIsIdAllowed(task->id) && !CoreTaskHasFlags(task, 1)) {
            update = &((const VtblEntry *)task->vtable)[2];
            ((void (*)(void *))update->fn)((u8 *)task + update->delta);
        }
    }
    g_taskList->iterating = 0;
}
