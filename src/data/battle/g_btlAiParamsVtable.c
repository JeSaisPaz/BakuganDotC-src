// bdc 0x08af6280 g_btlAiParamsVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParamsVtable = {
    {0}, { .pfn = (void *)BtlAiParamsDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
