// bdc 0x0889b33c BtlAiComScriptCacheAcquire
#include "bdc.h"

/* Returns the shared rule-script cache `g_btlAiComScriptCache` (32 records of 0x10 bytes),
   allocating it on the first call (users count 0) from the low heap (`MemAlloc` 0x200 bytes
   under `MemLock`, previous placement policy restored) and constructing the records with
   `CxxVecNewSimple` (element ctor `BtlAiComScriptCacheEntryCtor`); the cache stays NULL when
   the allocation fails. Increments `g_btlAiComScriptCacheUsers` on every call. Called by
   `BtlAiCtor` (stored at `ai+0x2c4`). */
void *BtlAiComScriptCacheAcquire(void)
{
    if (g_btlAiComScriptCacheUsers == 0) {
        bool fromLow;
        BtlAiComScript *cache;

        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        cache = MemAlloc(0x200, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (cache != NULL) {
            CxxVecNewSimple(cache, 0x20, 0x10, (void *)BtlAiComScriptCacheEntryCtor);
        }
        g_btlAiComScriptCache = cache;
    }
    g_btlAiComScriptCacheUsers++;
    return g_btlAiComScriptCache;
}
