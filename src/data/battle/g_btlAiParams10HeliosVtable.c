// bdc 0x08af6838 g_btlAiParams10HeliosVtable
#include "bdc.h"

__typeof__(MemberFnPtr[10]) g_btlAiParams10HeliosVtable = {
    {0}, { .pfn = (void *)BtlAiParams10HeliosDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C }, { .pfn = (void *)BtlAiParams10HeliosSlot9Return30 },
};
