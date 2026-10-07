// bdc 0x08a80388 g_btlAiMovePadConds
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_btlAiMovePadConds = {
    { .pfn = (void *)BtlAiPadStickForward }, { .pfn = (void *)BtlAiPadPress04 },
    { .pfn = (void *)BtlAiPadStickDirC000 }, { .pfn = (void *)BtlAiPadStickDir4000 },
    { .pfn = (void *)BtlAiPadStickBack }, { .pfn = (void *)BtlAiPadPressDash },
    { .pfn = (void *)BtlAiPadStickDir4000 }, { .pfn = (void *)BtlAiPadStickDirC000 },
    { .pfn = (void *)BtlAiPadPress04 },
};
