// bdc 0x08af6a38 g_btlAiParams26PiercianVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams26PiercianVtable = {
    {0}, { .pfn = (void *)BtlAiParams26PiercianDtor },
    { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParams26PiercianGetCounterChance },
    { .pfn = (void *)BtlAiParams26PiercianGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
