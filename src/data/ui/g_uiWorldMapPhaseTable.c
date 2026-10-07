// bdc 0x08a9eeb8 g_uiWorldMapPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[6]) g_uiWorldMapPhaseTable = {
    { .pfn = (void *)UiWorldMapPhaseLoad }, { .pfn = (void *)UiWorldMapPhaseIntro },
    { .pfn = (void *)UiWorldMapPhaseMain }, { .pfn = (void *)UiWorldMapPhaseClose },
    { .pfn = (void *)UiWorldMapNetPhaseMain }, { .pfn = (void *)UiWorldMapPhaseOptions },
};
