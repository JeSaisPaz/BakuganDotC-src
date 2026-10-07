// bdc 0x089cb470 ScriptOpTaskQuery
#include "bdc.h"

/* Queries a task: calls its virtual method in vtable slot 6 (`vtable+0x30` delta, `+0x34` fn) with
   one value and stores the result into a script variable; does nothing if no task has the id. */

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

int ScriptOpTaskQuery(Script *script)

{
  u32 id;
  u32 a;
  u32 *out;
  TaskObj *task;
  
  id = ScriptReadU32(script);
  a = ScriptReadU32(script);
  out = ScriptReadRef(script,2);
  task = (TaskObj *)CoreTaskFind(id);
  if (task != (TaskObj *)0x0) {
    TaskVEntry *e = task->vtable + 6;
    *out = ((u32 (*)(void *, u32))e->fn)((char *)task + e->delta, a);
  }
  return 0;
}
