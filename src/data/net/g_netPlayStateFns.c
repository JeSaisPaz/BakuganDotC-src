// bdc 0x08a50970 g_netPlayStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[9]) g_netPlayStateFns = {
    { .pfn = (void *)NetPlayState0Idle }, { .pfn = (void *)NetPlayState1Connect },
    { .pfn = (void *)NetPlayState2Lobby }, { .pfn = (void *)NetPlayState3HostLaunch },
    { .pfn = (void *)NetPlayState4ScanHosts }, { .pfn = (void *)NetPlayState5JoinWait },
    { .pfn = (void *)NetPlayState6JoinLaunch }, { .pfn = (void *)NetPlayState7Session },
    { .pfn = (void *)NetPlayStateAbort },
};
