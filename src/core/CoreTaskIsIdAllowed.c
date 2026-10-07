// bdc 0x089bfaa0 CoreTaskIsIdAllowed
#include "bdc.h"

/* Decides whether a task with the given `id` may run its update this frame. Returns 1 when `id` is
   0, when no exclusive task id is set (`g_taskExclusiveId` `== 0`), when `id` equals that
   exclusive id, or when `id` appears in the 8-entry `g_taskIdWhitelist`; otherwise 0. Called by
   `CoreTaskManagerUpdate` for every task and by another task-update caller. */
bool CoreTaskIsIdAllowed(s32 id)
{
    s32 i;

    if (id == 0 || g_taskExclusiveId == 0 || g_taskExclusiveId == (u32)id) {
        return true;
    }
    for (i = 0; i < 8; i++) {
        if (g_taskIdWhitelist[i] == id) {
            return true;
        }
    }
    return false;
}
