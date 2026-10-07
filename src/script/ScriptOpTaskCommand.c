// bdc 0x089cb3dc ScriptOpTaskCommand
#include "bdc.h"

/* Sends a command to a task: looks up the task by id (`CoreTaskFind`) and, if found, calls its
   virtual method in vtable slot 5 (`vtable+0x28` delta, `+0x2c` fn) with two values. */

int ScriptOpTaskCommand(Script *script)

{
  u32 id;
  u32 a;
  u32 b;
  CoreTask *task;
  
  id = ScriptReadU32(script);
  a = ScriptReadU32(script);
  b = ScriptReadU32(script);
  task = (CoreTask *)CoreTaskFind(id);
  if (task != NULL) {
    const VtblEntry *e = &((const VtblEntry *)task->vtable)[5];
    ((void (*)(void *, u32, u32))e->fn)((u8 *)task + e->delta, a, b);
  }
  return 0;
}
