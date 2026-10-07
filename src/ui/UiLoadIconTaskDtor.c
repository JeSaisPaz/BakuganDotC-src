// bdc 0x088089bc UiLoadIconTaskDtor
#include "bdc.h"

/* Destructor of the disc-access indicator task (id 2003): restores the vtable, chains to
   `CoreTaskDestroy`, frees on bit 0 of `flags`. */
void UiLoadIconTaskDtor(CoreTask *task, u32 flags)
{
  if (task != NULL) {
    task->vtable = &g_uiLoadIconTaskVtbl;
    CoreTaskDestroy(task, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, NULL, 0);
      MemUnlock();
    }
  }
}
