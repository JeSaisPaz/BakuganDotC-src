// bdc 0x08af6328 g_btlAiParams15AltairVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams15AltairVtable = {
    {0}, { .pfn = (void *)BtlAiParams15AltairDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
