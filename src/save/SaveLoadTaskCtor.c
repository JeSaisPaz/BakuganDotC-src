// bdc 0x0880a4dc SaveLoadTaskCtor
#include "bdc.h"

/* Constructor of the manual load task (task id 10010 = 0x271a, 0x18 bytes, vtable `0x08af1574`):
   allocates the 4-byte handler holder `0x08aac9c8`, clears profile word 1 (last save result),
   resets the step and makes sure the message window exists (`UiMsgWindowEnsure`). */

CoreTask *SaveLoadTaskCtor(CoreTask *task)

{
  SaveLoadTask *self = (SaveLoadTask *)task;
  bool fromLow;
  void **s;
  CoreTaskInit(task);
  task->vtable = g_saveLoadTaskVtbl;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  s = MemAlloc(sizeof(void *),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_saveLoadDialogSlot = s;
  memset(s,0,sizeof(void *));
  if (SaveHasProfile()) {
    SaveProfileSetWord(SaveGetProfile(),1,0);
  }
  self->step = 0;
  UiMsgWindowEnsure();
  return task;
}

