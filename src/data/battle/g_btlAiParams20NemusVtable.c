// bdc 0x08af6370 g_btlAiParams20NemusVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams20NemusVtable = {
    {0}, { .pfn = (void *)BtlAiParams20NemusDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
