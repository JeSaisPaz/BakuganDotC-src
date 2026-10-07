// bdc 0x0880b164 SaveSaveTaskDtor
#include "bdc.h"

/* Destructor of the manual save task (id 10020): frees the handler holder `0x08aac9d8`; if the save
   succeeded (profile word 1 == 1) runs `SaveSnapshotUpdate` (copies the save data block into the
   game's backup copy at `0x08abd925` when `0x08abe8f5` is set); chains to `CoreTaskDestroy`. */

void SaveSaveTaskDtor(CoreTask *task, u32 flags)

{
  SaveProfile *self;
  u32 word;
  
  if (task != (CoreTask *)0x0) {
    task->vtable = g_saveSaveTaskVtbl;
    if (g_saveSaveDialogSlot != (void **)0x0) {
      MemLock();
      MemFree(g_saveSaveDialogSlot,(char *)0x0,0);
      MemUnlock();
      g_saveSaveDialogSlot = (void **)0x0;
    }
    self = SaveGetProfile();
    word = SaveProfileGetWord(self,1);
    if (word == 1) {
      SaveSnapshotUpdate();
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

