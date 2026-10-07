// bdc 0x0889b3fc BtlAiComScriptCacheRelease
#include "bdc.h"

/* Drops one user of the shared rule-script cache `g_btlAiComScriptCache`
   (`g_btlAiComScriptCacheUsers`); when the count reaches 0 and the cache exists, destroys its 32
   records (`CxxVecDelete` with `BtlAiComScriptDtor`, no free by the helper), frees the block
   under `MemLock` and clears the pointer. Called by `BtlAiDtor`. */
void BtlAiComScriptCacheRelease(void)
{
    g_btlAiComScriptCacheUsers--;
    if (g_btlAiComScriptCacheUsers != 0 || g_btlAiComScriptCache == NULL) {
        return;
    }
    CxxVecDelete(g_btlAiComScriptCache, 0x20, sizeof(BtlAiComScript), BtlAiComScriptDtor, 0, 0);
    MemLock();
    MemFree(g_btlAiComScriptCache, NULL, 0);
    MemUnlock();
    g_btlAiComScriptCache = NULL;
}
