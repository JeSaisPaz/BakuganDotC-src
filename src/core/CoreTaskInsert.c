// bdc 0x089bfbb0 CoreTaskInsert
#include "bdc.h"

/* Inserts an already constructed task object into `g_taskList` at `priority` (ascending order)
   through `CoreListInsert`. Ten callers (task constructors/spawners), among them
   `UiLoadIconShow`. */
void CoreTaskInsert(void *task, s32 priority)
{
    CoreListInsert(g_taskList, task, priority);
}
