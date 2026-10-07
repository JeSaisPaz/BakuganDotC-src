// bdc 0x08a9a1f0 g_btlStageCamStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[3]) g_btlStageCamStateFns = {
    { .pfn = (void *)BtlStageCamStateStart }, { .pfn = (void *)BtlStageCamStatePlay },
    { .pfn = (void *)BtlStageCamStateEnd },
};
