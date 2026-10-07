// bdc 0x08ac560c g_sndBgmPlayerStateTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_sndBgmPlayerStateTable = {
    { .pfn = (void *)SndBgmPlayerStateIdle }, { .pfn = (void *)SndBgmPlayerStateUnload },
    { .pfn = (void *)SndBgmPlayerStateLoad }, { .pfn = (void *)SndBgmPlayerStatePrepare },
    { .pfn = (void *)SndBgmPlayerStateStart },
};
