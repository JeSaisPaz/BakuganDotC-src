// bdc 0x0880a4c0 SaveNopTaskUpdate
#include "bdc.h"

/* Update of the no-op save task id 10011 (`SaveNopTaskCtor`): removes and destroys the task
   immediately (`CoreTaskRemove`(task, 1)); the 0x1c-byte body does nothing else. */

void SaveNopTaskUpdate(CoreTask *task)

{
  CoreTaskRemove(task,true);
  return;
}

