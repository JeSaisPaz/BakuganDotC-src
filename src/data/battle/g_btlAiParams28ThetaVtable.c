// bdc 0x08af6b58 g_btlAiParams28ThetaVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams28ThetaVtable = {
    {0}, { .pfn = (void *)BtlAiParams28ThetaDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
