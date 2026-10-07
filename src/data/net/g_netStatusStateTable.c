// bdc 0x08a9cffc g_netStatusStateTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_netStatusStateTable = {
    { .pfn = (void *)NetStatusTaskStateInit }, { .pfn = (void *)NetStatusTaskStateIdle },
    { .pfn = (void *)NetStatusTaskStateShow }, { .pfn = (void *)NetStatusTaskStateVisible },
    { .pfn = (void *)NetStatusTaskStateHide },
};
