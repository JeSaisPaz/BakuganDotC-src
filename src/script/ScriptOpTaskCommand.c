// bdc 0x089cb3dc ScriptOpTaskCommand
#include "bdc.h"

/* Sends a command to a task: looks up the task by id (`CoreTaskFind`) and, if found, calls its
   virtual method in vtable slot 5 (`vtable+0x28` delta, `+0x2c` fn) with two values. */

typedef struct TaskVEntry {
  short delta;
  short pad;
  void *fn;
} TaskVEntry;

typedef struct TaskObj {
  s32 id;
  s32 pad[2];
  TaskVEntry *vtable;
} TaskObj;

int ScriptOpTaskCommand(Script *script)

{
  u32 id;
  u32 a;
  u32 b;
  TaskObj *task;
  
  id = ScriptReadU32(script);
  a = ScriptReadU32(script);
  b = ScriptReadU32(script);
  task = (TaskObj *)CoreTaskFind(id);
  if (task != (TaskObj *)0x0) {
    TaskVEntry *e = task->vtable + 5;
    ((void (*)(void *, u32, u32))e->fn)((char *)task + e->delta, a, b);
  }
  return 0;
}
