// bdc 0x0889b488 BtlAiComScriptCacheIndexOf
#include "bdc.h"

/* Returns the record index of `kind` in the rule-script cache (`BtlAiKindToScriptIndex`). */

s32 BtlAiComScriptCacheIndexOf(void *cache, s32 kind)
{
    (void)cache;
    return BtlAiKindToScriptIndex(kind);
}
