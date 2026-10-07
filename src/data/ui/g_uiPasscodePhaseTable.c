// bdc 0x08a9ce20 g_uiPasscodePhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiPasscodePhaseTable = {
    { .pfn = (void *)UiPasscodeLoadPhase }, { .pfn = (void *)UiPasscodeInitPhase },
    { .pfn = (void *)UiPasscodeMainPhase }, { .pfn = (void *)UiPasscodeExitPhase },
};
