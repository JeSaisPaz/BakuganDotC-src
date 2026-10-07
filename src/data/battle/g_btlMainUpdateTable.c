// bdc 0x08a66444 g_btlMainUpdateTable
#include "bdc.h"

__typeof__(MemberFnPtr[10]) g_btlMainUpdateTable = {
    { .pfn = (void *)BtlMainPhaseLoad }, { .pfn = (void *)BtlMainPhaseBattle },
    { .pfn = (void *)BtlMainPhaseFinish }, { .pfn = (void *)BtlMainPhaseExit },
    { .pfn = (void *)BtlMainPhaseTalk }, { .pfn = (void *)BtlMainPhaseCutIn },
    { .pfn = (void *)BtlMainPhaseSceneOnly }, { .pfn = (void *)BtlMainPhaseResume },
    { .pfn = (void *)BtlMainPhaseWaitHud }, { .pfn = (void *)BtlMainPhaseIntro },
};
