// bdc 0x08809174 SaveAutoLoadTaskCtor
#include "bdc.h"

/* Constructor of the save-data autoload task (task id 10009 = 0x2719, 0x18 bytes, vtable
   `0x08af1494`): `CoreTaskInit`, clears the state and clears profile flag `0x40000000` ('save
   data loaded'). */

CoreTask *SaveAutoLoadTaskCtor(CoreTask *task)

{
  SaveAutoLoadTask *self = (SaveAutoLoadTask *)task;

  CoreTaskInit(task);
  task->vtable = &g_saveAutoLoadTaskVtbl;
  self->handler = 0;
  self->step = 0;
  SaveProfileClearFlags(SaveGetProfile(),0x40000000);
  return task;
}
