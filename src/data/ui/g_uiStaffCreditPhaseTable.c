// bdc 0x08a9d078 g_uiStaffCreditPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiStaffCreditPhaseTable = {
    { .pfn = (void *)UiStaffCreditLoadPhase }, { .pfn = (void *)UiStaffCreditSetupPhase },
    { .pfn = (void *)UiStaffCreditMainPhase }, { .pfn = (void *)UiStaffCreditClosePhase },
};
