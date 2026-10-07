// bdc 0x08af6888 g_btlAiParams09HeliosVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams09HeliosVtable = {
    {0}, { .pfn = (void *)BtlAiParams09HeliosDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
