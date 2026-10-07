// bdc 0x08a9d360 g_uiNetMenuPhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiNetMenuPhaseFns = {
    { .pfn = (void *)UiNetMenuInitPhase }, { .pfn = (void *)UiNetMenuSetupPhase },
    { .pfn = (void *)UiNetMenuMainPhase }, { .pfn = (void *)UiNetMenuExitPhase },
};
