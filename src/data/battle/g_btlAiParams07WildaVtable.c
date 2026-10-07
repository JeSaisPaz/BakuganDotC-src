// bdc 0x08af6ba0 g_btlAiParams07WildaVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams07WildaVtable = {
    {0}, { .pfn = (void *)BtlAiParams07WildaDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
