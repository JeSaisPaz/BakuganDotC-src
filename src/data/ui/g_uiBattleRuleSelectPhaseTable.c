// bdc 0x08a9d4e0 g_uiBattleRuleSelectPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_uiBattleRuleSelectPhaseTable = {
    { .pfn = (void *)UiBattleRuleSelectWaitPhase }, { .pfn = (void *)UiBattleRuleSelectSetupPhase },
    { .pfn = (void *)UiBattleRuleSelectMainPhase }, { .pfn = (void *)UiBattleRuleSelectExitPhase },
    { .pfn = (void *)UiBattleRuleSelectNetPhase },
};
