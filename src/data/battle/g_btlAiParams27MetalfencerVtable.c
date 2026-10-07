// bdc 0x08af68d0 g_btlAiParams27MetalfencerVtable
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiParams27MetalfencerVtable = {
    {0}, { .pfn = (void *)BtlAiParams27MetalfencerDtor },
    { .pfn = (void *)BtlAiParamsGetSidestepMode },
    { .pfn = (void *)BtlAiParamsGetBlockedMoveChance },
    { .pfn = (void *)BtlAiParamsGetCounterChance }, { .pfn = (void *)BtlAiParamsGetReactChance },
    { .pfn = (void *)BtlAiParamsGetChance9E4 }, { .pfn = (void *)BtlAiParamsReturn40 },
    { .pfn = (void *)BtlAiParamsGetValue3C },
};
