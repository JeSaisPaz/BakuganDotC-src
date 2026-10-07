// bdc 0x0880a5a0 SaveLoadTaskDtor
#include "bdc.h"

/* Destructor of the manual load task (id 10010): frees the handler holder `0x08aac9c8`, chains to
   `CoreTaskDestroy` and frees the object when bit 0 of `flags` is set. */

void SaveLoadTaskDtor(CoreTask *task, u32 flags)

{
  if (task != (CoreTask *)0x0) {
    task->vtable = g_saveLoadTaskVtbl;
    if (g_saveLoadDialogSlot != (void **)0x0) {
      MemLock();
      MemFree(g_saveLoadDialogSlot,(char *)0x0,0);
      MemUnlock();
      g_saveLoadDialogSlot = (void **)0x0;
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

