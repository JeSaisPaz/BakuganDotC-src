// bdc 0x0889b4d0 BtlAiComScriptCacheGet
#include "bdc.h"

/* Returns the rule-script record (`BtlAiScriptCacheEntry`) of `kind` in the cache. Called by
   `BtlAiSetupKind`. */
void *BtlAiComScriptCacheGet(void *cache, s32 kind)
{
    BtlAiScriptCacheEntry *entries = cache;

    return &entries[BtlAiComScriptCacheIndexOf(cache, kind)];
}
