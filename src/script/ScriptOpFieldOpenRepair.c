// bdc 0x0880ea18 ScriptOpFieldOpenRepair
#include "bdc.h"

/* Script opcode: reads u16 `value` and, when the field task (id 500, `GameFieldCtor`) exists
   (`CoreTaskFind`), opens the repair screen with `GameFieldOpenRepairScreen``(field, value)`.
   Returns 0. */

int ScriptOpFieldOpenRepair(Script *script)

{
  u32 value;
  void *task;
  
  value = ScriptReadU16(script);
  task = CoreTaskFind(500);
  if (task != (void *)0x0) {
    GameFieldOpenRepairScreen(task,value);
  }
  return 0;
}

