// bdc 0x0889b1b4 BtlAiComScriptFree
#include "bdc.h"

/* Frees the contents of a CPU-AI rule script record (`BtlAiComScript`, filled by
   `BtlAiLoadComScript`): for each of the `groupCount` groups, frees its rule array's
   `CxxVecBlock` when present and clears the group's `records` and `count` (only `count` when
   there is no array); then frees the group table and clears `groups` and `groupCount`. Does
   nothing when there is no group table. */
void BtlAiComScriptFree(void *rules)
{
    BtlAiComScript *script = (BtlAiComScript *)rules;
    BtlAiRuleGroup *groups = script->groups;
    s32 i;

    if (groups == NULL) {
        return;
    }
    for (i = 0; i < script->groupCount; i++) {
        BtlAiRuleRecord *records = groups[i].records;

        if (records == NULL) {
            groups[i].count = 0;
        } else {
            MemLock();
            MemFree((CxxVecBlock *)((u8 *)records - __builtin_offsetof(CxxVecBlock, elements)), NULL, 0);
            MemUnlock();
            script->groups[i].records = NULL;
            script->groups[i].count = 0;
        }
        groups = script->groups;
    }
    MemLock();
    MemFree(groups, NULL, 0);
    MemUnlock();
    script->groups = NULL;
    script->groupCount = 0;
}
