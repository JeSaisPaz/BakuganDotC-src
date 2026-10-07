// bdc 0x088093cc SaveDeleteTaskCtor
#include "bdc.h"

/* Constructor of the savedata delete task (id 10030 = 0x272e, 0x14 bytes, vtable `0x08af14cc`):
   allocates the 4-byte handler holder `0x08aac9c0`, clears profile word 1 and the step.
   `SaveDeleteTaskUpdate` runs savedata request 2, the delete list (mode 7 `LISTALLDELETE` in the
   vendored SDK enum) that `SaveSaveTaskUpdate` and `SaveLanguageTaskUpdate` also open when the
   memory stick is full. */

CoreTask *SaveDeleteTaskCtor(CoreTask *task)

{
  bool hadLow;
  void **cell;
  SaveProfile *self;

  CoreTaskInit(task);
  task->vtable = &g_saveDeleteTaskVtable;
  MemLock();
  hadLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  cell = MemAlloc(4, (char *)0x0, 0);
  MemSetAllocFromLow(hadLow);
  MemUnlock();
  g_saveDeleteDialogSlot = cell;
  memset(cell, 0, 4);
  if (SaveHasProfile()) {
    self = SaveGetProfile();
    SaveProfileSetWord(self, 1, 0);
  }
  ((SaveDeleteTask *)task)->step = 0;
  return task;
}
