// bdc 0x089fe528 IoPacLoaderTaskDtor
#include "bdc.h"

/* Destructor of the `.pac` loader task (task id 10110, vtable `0x08af591c`, created by
   `IoPacLoaderTaskCtor`): destroys the package loader singleton (`IoPacLoaderDestroy`), runs
   `CoreTaskDestroy` and frees the task when `flags & 1`. */

void IoPacLoaderTaskDtor(CoreTask *task, u32 flags)

{
  if (task != (CoreTask *)0x0) {
    task->vtable = g_ioPacLoaderTaskVtbl;
    IoPacLoaderDestroy();
    CoreTaskDestroy(task,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(task,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

