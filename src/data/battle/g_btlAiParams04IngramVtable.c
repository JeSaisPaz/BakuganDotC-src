// bdc 0x08af65f8 g_btlAiParams04IngramVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams04IngramVtable = {
    {0}, { .pfn = (void *)BtlAiParams04IngramDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
