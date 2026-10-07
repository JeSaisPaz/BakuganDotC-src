// bdc 0x089cb920 ScriptMngShutdown
#include "bdc.h"

/* Destructor of the script manager: frees the script variable block (`ScriptVarsFree`) and the
   loaded script package (`ScriptPackageFree`), then releases `mng` itself when bit 0 of `flags`
   is set (GCC 2.x deleting-destructor convention; callers pass 3). */

void ScriptMngShutdown(void *mng, u32 flags)

{
  if (mng != (void *)0x0) {
    ScriptVarsFree();
    ScriptPackageFree();
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(mng,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

