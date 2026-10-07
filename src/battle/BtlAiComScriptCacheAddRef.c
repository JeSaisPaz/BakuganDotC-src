// bdc 0x0889b4a4 BtlAiComScriptCacheAddRef
#include "bdc.h"

/* References the rule script of `kind` in the cache (`BtlAiComScriptAddRef` on its
   `BtlAiScriptCacheEntry` record, loading it on first use). Called by `BtlAiSetupKind`. */
void BtlAiComScriptCacheAddRef(void *cache, s32 kind)
{
    BtlAiScriptCacheEntry *entries = cache;
    s32 id = BtlAiComScriptCacheIndexOf(cache, kind);

    BtlAiComScriptAddRef((u8 *)&entries[id], id);
}
