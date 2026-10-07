// bdc 0x089eead0 GfxFaderTaskCtor
#include "bdc.h"

/* Constructor of the screen fader task (task id 10060 / 0x274c, object size 0x10, vtable
   `0x08af57e4`) created by `CoreTaskNewById`: base `CoreTaskInit` and the vtable. */

CoreTask *GfxFaderTaskCtor(CoreTask *task)

{
  CoreTaskInit(task);
  task->vtable = (const void *)&g_gfxFaderTaskVtbl;
  return task;
}

