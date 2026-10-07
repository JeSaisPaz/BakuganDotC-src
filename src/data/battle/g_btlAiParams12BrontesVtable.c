// bdc 0x08af6400 g_btlAiParams12BrontesVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams12BrontesVtable = {
    {0}, { .pfn = (void *)BtlAiParams12BrontesDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
