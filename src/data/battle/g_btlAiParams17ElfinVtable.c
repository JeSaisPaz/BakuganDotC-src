// bdc 0x08af6520 g_btlAiParams17ElfinVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams17ElfinVtable = {
    {0}, { .pfn = (void *)BtlAiParams17ElfinDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
