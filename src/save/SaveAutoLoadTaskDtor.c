// bdc 0x088091c4 SaveAutoLoadTaskDtor
#include "bdc.h"

/* Destructor of the save-data autoload task (id 10009): restores the class vtable, chains to
   `CoreTaskDestroy` and frees the object when bit 0 of `flags` is set. */
void SaveAutoLoadTaskDtor(CoreTask *task, u32 flags)
{
  if (task != NULL) {
    task->vtable = &g_saveAutoLoadTaskVtbl;
    CoreTaskDestroy(task, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, NULL, 0);
      MemUnlock();
    }
  }
}
