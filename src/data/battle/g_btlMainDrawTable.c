// bdc 0x08a66494 g_btlMainDrawTable
#include "bdc.h"

__typeof__(MemberFnPtr[10]) g_btlMainDrawTable = {
    { .pfn = (void *)BtlMainDrawLoad }, { .pfn = (void *)BtlMainDrawScene },
    { .pfn = (void *)BtlMainDrawScene }, { .pfn = (void *)BtlMainDrawExit },
    { .pfn = (void *)BtlMainDrawTalk }, { .pfn = (void *)BtlMainDrawCutIn },
    { .pfn = (void *)BtlMainDrawScene }, { .pfn = (void *)BtlMainDrawScene },
    { .pfn = (void *)BtlMainDrawScene }, { .pfn = (void *)BtlMainDrawScene },
};
