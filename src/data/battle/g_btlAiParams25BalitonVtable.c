// bdc 0x08af63b8 g_btlAiParams25BalitonVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams25BalitonVtable = {
    {0}, { .pfn = (void *)BtlAiParams25BalitonDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParams25BalitonGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
