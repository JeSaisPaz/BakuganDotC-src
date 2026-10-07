// bdc 0x08af6568 g_btlAiParams11ElicoVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams11ElicoVtable = {
    {0}, { .pfn = (void *)BtlAiParams11ElicoDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
