// bdc 0x089fe4ec IoPacLoaderTaskCtor
#include "bdc.h"

/* Constructor of the `.pac` package loader task (task id 10110 = 0x277e, object size 0x10, built by
   `CoreTaskNewById`): runs `CoreTaskInit`, installs vtable `0x08af591c` and creates the loader
   singleton `g_ioPacLoader` (`IoPacLoaderCreate`). Returns `task`. */

CoreTask *IoPacLoaderTaskCtor(CoreTask *task)

{
  CoreTaskInit(task);
  task->vtable = g_ioPacLoaderTaskVtbl;
  IoPacLoaderCreate();
  return task;
}

