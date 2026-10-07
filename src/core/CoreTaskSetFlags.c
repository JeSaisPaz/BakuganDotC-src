// bdc 0x089bf2b0 CoreTaskSetFlags
#include "bdc.h"

/* Sets the bits of `mask` in the task's flag word (`task->flags |= mask`): bit 0 disables its
   update, bit 1 hides it (see `CoreTask`, `CoreTaskHasFlags`). */
void CoreTaskSetFlags(CoreTask *task, u32 mask)
{
    task->flags |= mask;
}
