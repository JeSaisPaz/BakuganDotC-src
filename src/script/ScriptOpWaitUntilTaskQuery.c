// bdc 0x089cb508 ScriptOpWaitUntilTaskQuery
#include "bdc.h"

/* Blocks until a comparison of the task's slot-6 query result `r` (see `ScriptOpTaskQuery`)
   against a value holds: returns 2 (wait) while the test selected by `cmp` is true, 0 once it
   is false, if `cmp >= 6`, or if the task does not exist. */

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

int ScriptOpWaitUntilTaskQuery(Script *script)

{
  u32 taskId;
  u32 arg;
  u32 cmp;
  s32 value;
  s32 r;
  TaskObj *task;
  TaskVEntry *e;
  int result;

  result = 0;
  taskId = ScriptReadU32(script);
  arg = ScriptReadU32(script);
  cmp = ScriptReadU32(script);
  value = (s32)ScriptReadU32(script);
  task = (TaskObj *)CoreTaskFind(taskId);
  if (task != (TaskObj *)0x0) {
    result = 0;
    e = task->vtable + 6;
    r = (s32)((u32 (*)(void *, u32))e->fn)((char *)task + e->delta, arg);
    if (cmp < 6) {
      switch (cmp) {
      case 1:
        if (value < r) result = 2;
        break;
      case 2:
        if (!(value < r)) result = 2;
        break;
      case 3:
        if (!(r < value)) result = 2;
        break;
      case 4:
        if (value != r) result = 2;
        break;
      case 5:
        if (value == r) result = 2;
        break;
      default:
        if (r < value) result = 2;
        break;
      }
    }
  }
  return result;
}
