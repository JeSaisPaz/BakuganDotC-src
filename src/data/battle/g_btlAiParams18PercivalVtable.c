// bdc 0x08af6918 g_btlAiParams18PercivalVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams18PercivalVtable = {
    {0}, { .pfn = (void *)BtlAiParams18PercivalDtor },
    { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
