// bdc 0x0880948c SaveDeleteTaskDtor
#include "bdc.h"

/* Destructor of the savedata delete task (id 10030, `SaveDeleteTaskCtor`): frees the handler
   holder `0x08aac9c0`, chains to `CoreTaskDestroy`, frees on bit 0 of `flags`. */

void SaveDeleteTaskDtor(CoreTask *task, u32 flags)

{
  if (task != (CoreTask *)0x0) {
    task->vtable = &g_saveDeleteTaskVtable;
    if (g_saveDeleteDialogSlot != (void **)0x0) {
      MemLock();
      MemFree(g_saveDeleteDialogSlot,(char *)0x0,0);
      MemUnlock();
      g_saveDeleteDialogSlot = (void **)0x0;
    }
    CoreTaskDestroy(task,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

