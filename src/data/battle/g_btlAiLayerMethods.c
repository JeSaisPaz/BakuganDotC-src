// bdc 0x08a802e0 g_btlAiLayerMethods
#include "bdc.h"

__typeof__(MemberFnPtr[10]) g_btlAiLayerMethods = {
    { .pfn = (void *)BtlAiGuardLayerCheck }, { .pfn = (void *)BtlAiGuardLayerRun },
    { .pfn = (void *)BtlAiFollowLayerCheck }, { .pfn = (void *)BtlAiFollowLayerRun },
    { .pfn = (void *)BtlAiSeekItemCheck }, { .pfn = (void *)BtlAiSeekItemRun },
    { .pfn = (void *)BtlAiWanderLayerCheck }, { .pfn = (void *)BtlAiWanderRun },
    { .pfn = (void *)BtlAiThinkLayerCheck }, { .pfn = (void *)BtlAiThink },
};
