// bdc 0x08af6760 g_btlAiParams03IngramVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams03IngramVtable = {
    {0}, { .pfn = (void *)BtlAiParams03IngramDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
