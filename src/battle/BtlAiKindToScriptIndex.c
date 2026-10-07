// bdc 0x0889b480 BtlAiKindToScriptIndex
#include "bdc.h"

/* Returns `kind - 1`, the rule-script / parameter index of a unit kind. Used by
   `BtlAiComScriptCacheIndexOf` and `BtlAiCreateKindParamSet`. */
s32 BtlAiKindToScriptIndex(s32 kind)
{
    return kind - 1;
}
