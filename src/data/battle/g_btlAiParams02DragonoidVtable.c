// bdc 0x08af67f0 g_btlAiParams02DragonoidVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams02DragonoidVtable = {
    {0}, { .pfn = (void *)BtlAiParams02DragonoidDtor },
    { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
