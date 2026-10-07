// bdc 0x08a9b1b0 g_uiLoadingStateTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiLoadingStateTable = {
    { .pfn = (void *)UiLoadingStateInit }, { .pfn = (void *)UiLoadingStateSetup },
    { .pfn = (void *)UiLoadingStateWaitDisc }, { .pfn = (void *)UiLoadingStateIdle },
    { .pfn = (void *)UiLoadingStateClose },
};
