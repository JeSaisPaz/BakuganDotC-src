// bdc 0x08af6688 g_btlAiParams14HadesVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams14HadesVtable = {
    {0}, { .pfn = (void *)BtlAiParams14HadesDtor }, { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
