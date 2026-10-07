// bdc 0x08a9aac4 g_btlDemoScenePlayerStateTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_btlDemoScenePlayerStateTable = {
    { .pfn = (void *)BtlDemoScenePlayerStateStart }, { .pfn = (void *)BtlDemoScenePlayerStateLoad },
    { .pfn = (void *)BtlDemoScenePlayerStatePlay }, { .pfn = (void *)BtlDemoScenePlayerStateDone },
};
