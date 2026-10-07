// bdc 0x08a9d900 g_uiCardEquipPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiCardEquipPhaseTable = {
    { .pfn = (void *)UiCardEquipPhaseStart }, { .pfn = (void *)UiCardEquipPhaseLoad },
    { .pfn = (void *)UiCardEquipPhaseMain }, { .pfn = (void *)UiCardEquipPhaseOptions },
    { .pfn = (void *)UiCardEquipPhaseClose },
};
