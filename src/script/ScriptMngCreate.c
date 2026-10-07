// bdc 0x089cb804 ScriptMngCreate
#include "bdc.h"

/* Creates the script manager singleton once: allocates a 1-byte object from the low end of the heap
   (under `MemLock`, saving/restoring the placement policy with
   `MemIsAllocFromLow`/`MemSetAllocFromLow`), runs `ScriptMngInit` on it and stores it in
   `g_scriptMng`. Does nothing if the singleton already exists; a failed allocation leaves it
   NULL. */

void ScriptMngCreate(void)

{
  void *result;
  bool fromLow;
  void *mng;
  
  result = g_scriptMng;
  if (g_scriptMng == (void *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mng = MemAlloc(1,(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    result = (void *)0x0;
    if (mng != (void *)0x0) {
      ScriptMngInit(mng);
      result = mng;
    }
  }
  g_scriptMng = result;
  return;
}

