// bdc 0x089eeb08 GfxFaderTaskDtor
#include "bdc.h"

/* Destructor of the fader updater task (id 0x274c, vtable `0x08af57e4`): `CoreTaskDestroy`, frees
   itself when `flags & 1`. */
void GfxFaderTaskDtor(CoreTask *task, u32 flags)
{
  if (task != NULL) {
    task->vtable = &g_gfxFaderTaskVtbl;
    CoreTaskDestroy(task, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, NULL, 0);
      MemUnlock();
    }
  }
}
