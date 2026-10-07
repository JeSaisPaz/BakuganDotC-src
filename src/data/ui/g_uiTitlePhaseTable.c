// bdc 0x08a9d3f8 g_uiTitlePhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[7]) g_uiTitlePhaseTable = {
    { .pfn = (void *)UiTitleLoadAssetsPhase }, { .pfn = (void *)UiTitleIntroPhase },
    { .pfn = (void *)UiTitlePressStartPhase }, { .pfn = (void *)UiTitleChoicePhase },
    { .pfn = (void *)UiTitleLoadPhase }, { .pfn = (void *)UiTitleAttractPhase },
    { .pfn = (void *)UiTitleClosePhase },
};
