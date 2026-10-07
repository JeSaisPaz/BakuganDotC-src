// bdc 0x08a9bc08 g_uiUpgradePhaseFns
#include "bdc.h"

__typeof__(MemberFnPtr[4]) g_uiUpgradePhaseFns = {
    { .pfn = (void *)UiUpgradeStartPhase }, { .pfn = (void *)UiUpgradeSetupPhase },
    { .pfn = (void *)UiUpgradeMainPhase }, { .pfn = (void *)UiUpgradeExitPhase },
};
