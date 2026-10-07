// bdc 0x08af6640 g_btlAiParams31FortressVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams31FortressVtable = {
    {0}, { .pfn = (void *)BtlAiParams31FortressDtor },
    { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParams31FortressGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
