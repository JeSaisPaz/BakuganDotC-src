// bdc 0x08a9f388 g_uiMainMenuPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[7]) g_uiMainMenuPhaseTable = {
    { .pfn = (void *)UiMainMenuPhaseLoad }, { .pfn = (void *)UiMainMenuPhaseIntro },
    { .pfn = (void *)UiMainMenuPhaseMain }, { .pfn = (void *)UiMainMenuPhaseBattleModeSelect },
    { .pfn = (void *)UiMainMenuPhaseBattleRuleSelect },
    { .pfn = (void *)UiMainMenuPhaseNetConnect }, { .pfn = (void *)UiMainMenuPhaseClose },
};
