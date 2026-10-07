// bdc 0x08a9f818 g_uiBattleModeSelectPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiBattleModeSelectPhaseTable = {
    { .pfn = (void *)UiBattleModeSelectPhaseStart }, { .pfn = (void *)UiBattleModeSelectPhaseLoad },
    { .pfn = (void *)UiBattleModeSelectPhaseMain }, { .pfn = (void *)UiBattleModeSelectPhaseClose },
};
