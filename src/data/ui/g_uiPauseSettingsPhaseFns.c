// bdc 0x08a9f698 g_uiPauseSettingsPhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiPauseSettingsPhaseFns = {
    { .pfn = (void *)UiPauseSettingsPhaseLoad }, { .pfn = (void *)UiPauseSettingsPhaseIntro },
    { .pfn = (void *)UiPauseSettingsPhaseMain }, { .pfn = (void *)UiPauseSettingsPhaseClose },
};
