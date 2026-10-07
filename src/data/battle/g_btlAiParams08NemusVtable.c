// bdc 0x08af6960 g_btlAiParams08NemusVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams08NemusVtable = {
    {0}, { .pfn = (void *)BtlAiParams08NemusDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
