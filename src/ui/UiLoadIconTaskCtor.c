// bdc 0x0880897c UiLoadIconTaskCtor
#include "bdc.h"

/* Constructor of the disc-access indicator task (task id 2003 = 0x7d3, 0x18 bytes, vtable
   `g_uiLoadIconTaskVtbl`): clears the busy-frame counter and the visible byte. */

CoreTask *UiLoadIconTaskCtor(CoreTask *task)
{
  UiLoadIconTask *self = (UiLoadIconTask *)task;
  CoreTaskInit(task);
  task->vtable = &g_uiLoadIconTaskVtbl;
  self->visible = 0;
  self->busyFrames = 0;
  return task;
}
