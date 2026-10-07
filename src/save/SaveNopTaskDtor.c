// bdc 0x0880a44c SaveNopTaskDtor
#include "bdc.h"

/* Destructor of the no-op save task id 10011 (`SaveNopTaskCtor`): restores the vtable, chains to
   `CoreTaskDestroy`, frees on bit 0 of `flags`. */
void SaveNopTaskDtor(CoreTask *task, u32 flags)
{
  if (task != NULL) {
    task->vtable = &g_saveNopTaskVtbl;
    CoreTaskDestroy(task, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, NULL, 0);
      MemUnlock();
    }
  }
}
