// bdc 0x08a9d5f8 g_uiEquipPhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[7]) g_uiEquipPhaseFns = {
    { .pfn = (void *)UiEquipFadeInPhase }, { .pfn = (void *)UiEquipSetupPhase },
    { .pfn = (void *)UiEquipMainPhase }, { .pfn = (void *)UiEquipCardEquipPhase },
    { .pfn = (void *)UiEquipClosePhase }, { .pfn = (void *)UiEquipNetMainPhase },
    { .pfn = (void *)UiEquipPhase06Nop },
};
