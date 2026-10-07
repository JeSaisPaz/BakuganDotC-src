// bdc 0x08af6be8 g_btlAiParams32WiredVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams32WiredVtable = {
    {0}, { .pfn = (void *)BtlAiParams32WiredDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
