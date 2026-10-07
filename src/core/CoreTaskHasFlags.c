// bdc 0x089bf294 CoreTaskHasFlags
#include "bdc.h"

/* Returns whether every bit of `mask` is set in the task's flag word. The task update and draw
   loops use it to skip a task: `CoreTaskManagerUpdate` tests mask 1 (update disabled) and
   `CoreTaskManagerDraw` tests mask 2 (draw hidden). */
bool CoreTaskHasFlags(void *task, u32 mask)
{
    const CoreTask *t = task;

    return (t->flags & mask) == mask;
}
