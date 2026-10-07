// bdc 0x089bfa84 CoreTaskSetExclusiveId
#include "bdc.h"

/* Sets the exclusive task id `g_taskExclusiveId` and returns the previous value. While it is
   non-zero, `CoreTaskIsIdAllowed` lets only that task (plus id 0 and the whitelist) run its
   update, so the game behind a modal task freezes; `CoreTaskRemove` clears it when the task
   being removed has that id. */
u32 CoreTaskSetExclusiveId(u32 id)
{
    u32 previous = g_taskExclusiveId;
    g_taskExclusiveId = id;
    return previous;
}
