// bdc 0x08a9d3b8 g_uiTitleMenuPhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiTitleMenuPhaseFns = {
    { .pfn = (void *)UiTitleMenuLoadPhase }, { .pfn = (void *)UiTitleMenuWaitPhase },
    { .pfn = (void *)UiTitleMenuMainPhase }, { .pfn = (void *)UiTitleMenuExitPhase },
};
