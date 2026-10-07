// bdc 0x089c9ffc ScriptOpEnsureTask
#include "bdc.h"

/* Makes sure a task exists: reads a u16 task id and, if `CoreTaskExists` says no task with that
   id is alive, creates it with `CoreTaskCreate``(id, 100)` (the task factory, default priority
   100). */

int ScriptOpEnsureTask(Script *script)

{
  u32 id;
  s32 exists;
  
  id = ScriptReadU16(script);
  exists = CoreTaskExists(id);
  if (exists != 1) {
    CoreTaskCreate(id,100);
  }
  return 0;
}

