// bdc 0x08af6448 g_btlAiParams16DragonoidVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams16DragonoidVtable = {
    {0}, { .pfn = (void *)BtlAiParams16DragonoidDtor },
    { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
