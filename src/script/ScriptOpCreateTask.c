// bdc 0x089cafb4 ScriptOpCreateTask
#include "bdc.h"

/* Creates a task unconditionally through the secondary task factory:
   `CoreTaskCreateDefault``(id, arg)` with a u16 task id and a u32 argument read from the
   operands. Returns 0. */

int ScriptOpCreateTask(Script *script)

{
  u32 id;
  u32 arg;

  id = ScriptReadU16(script);
  arg = ScriptReadU32(script);
  CoreTaskCreateDefault(id,(void *)(uintptr_t)arg);
  return 0;
}
