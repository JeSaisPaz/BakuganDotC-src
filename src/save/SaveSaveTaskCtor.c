// bdc 0x0880b0a4 SaveSaveTaskCtor
#include "bdc.h"

/* Constructor of the manual save task (task id 10020 = 0x2724, 0x18 bytes, vtable `0x08af15e4`):
   allocates the 4-byte handler holder `0x08aac9d8`, clears profile word 1 and the step. */

CoreTask *SaveSaveTaskCtor(CoreTask *task)

{
  bool hadLow;
  void **cell;
  SaveProfile *self;

  CoreTaskInit(task);
  task->vtable = g_saveSaveTaskVtbl;
  MemLock();
  hadLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  cell = MemAlloc(4, (char *)0x0, 0);
  MemSetAllocFromLow(hadLow);
  MemUnlock();
  g_saveSaveDialogSlot = cell;
  memset(cell, 0, 4);
  if (SaveHasProfile()) {
    self = SaveGetProfile();
    SaveProfileSetWord(self, 1, 0);
  }
  ((SaveSaveTask *)task)->step = 0;
  return task;
}
