// bdc 0x08900090 BtlAppearDemoDtor
#include "bdc.h"

/* Destructor of the Bakugan appear demo task (task id 0x67): reinstalls its vtable
   `g_btlAppearDemoVtbl`, runs the base demo destructor `BtlDemoDtor` (without freeing) and
   frees the task when bit 0 of `flags` is set. Does nothing for NULL. */

void BtlAppearDemoDtor(CoreTask *task, u32 flags)
{
  if (task != NULL) {
    task->vtable = &g_btlAppearDemoVtbl;
    BtlDemoDtor(task, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task, NULL, 0);
      MemUnlock();
    }
  }
}
