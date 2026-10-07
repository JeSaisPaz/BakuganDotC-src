// bdc 0x0889b294 BtlAiComScriptDtor
#include "bdc.h"

/* Destructor of a CPU-AI rule script record (filled by `BtlAiLoadComScript`): frees its
   contents with `BtlAiComScriptFree`, then frees the record itself when `flags & 1`. Does
   nothing for NULL. Element destructor used by `BtlAiComScriptCacheRelease`. */
void BtlAiComScriptDtor(void *rules, u32 flags)
{
    if (rules == NULL) {
        return;
    }
    BtlAiComScriptFree(rules);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(rules, NULL, 0);
        MemUnlock();
    }
}
