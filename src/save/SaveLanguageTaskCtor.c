// bdc 0x0880a8f8 SaveLanguageTaskCtor
#include "bdc.h"

/* Constructor of the language-file save task (task id 10021 = 0x2725, 0x18 bytes, vtable
   `0x08af15ac`): allocates the handler holder `0x08aac9d0`, clears profile word 1, makes sure the
   message window exists (`UiMsgWindowEnsure`) and resets the step. */

SaveLanguageTask *SaveLanguageTaskCtor(SaveLanguageTask *task)

{
  bool prevLow;
  void **slot;

  CoreTaskInit(&task->base);
  task->base.vtable = &g_saveLanguageTaskVtbl;
  MemLock();
  prevLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  slot = MemAlloc(4, NULL, 0);
  MemSetAllocFromLow(prevLow);
  MemUnlock();
  g_saveLanguageDialogSlot = slot;
  memset(slot, 0, 4);
  if (SaveHasProfile()) {
    SaveProfileSetWord(SaveGetProfile(), 1, 0);
  }
  UiMsgWindowEnsure();
  task->step = 0;
  return task;
}
