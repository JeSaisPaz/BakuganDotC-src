// bdc 0x0880a9c0 SaveLanguageTaskDtor
#include "bdc.h"

/* Destructor of the language-file save task (id 10021): frees the handler holder `0x08aac9d0`,
   chains to `CoreTaskDestroy`, frees on bit 0 of `flags`. */

void SaveLanguageTaskDtor(CoreTask *task, u32 flags)

{
  if (task != (CoreTask *)0x0) {
    task->vtable = &g_saveLanguageTaskVtbl;
    if (g_saveLanguageDialogSlot != (void **)0x0) {
      MemLock();
      MemFree(g_saveLanguageDialogSlot,(char *)0x0,0);
      MemUnlock();
      g_saveLanguageDialogSlot = (void **)0x0;
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

