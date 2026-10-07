// bdc 0x088a13d4 GfxLensFlareTaskDtor
#include "bdc.h"

/* Destructor of the lens-flare task (id 482): restores the vtable, chains to `CoreTaskDestroy`,
   frees on bit 0 of `flags`. */
void GfxLensFlareTaskDtor(CoreTask *task, u32 flags)
{
  if (task != NULL) {
    task->vtable = &g_gfxLensFlareTaskVtbl;
    CoreTaskDestroy(task, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, NULL, 0);
      MemUnlock();
    }
  }
}
