// bdc 0x08af64d8 g_btlAiParams05ElfinVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams05ElfinVtable = {
    {0}, { .pfn = (void *)BtlAiParams05ElfinDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
