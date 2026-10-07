// bdc 0x089bf2c0 CoreTaskClearFlags
#include "bdc.h"

/* Clears the bits of `mask` in the task's flag word; undoes `CoreTaskSetFlags`. */
void CoreTaskClearFlags(CoreTask *task, u32 mask)
{
    task->flags &= ~mask;
}
