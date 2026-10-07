// bdc 0x08a9d19c g_uiBattleRecordPhaseTable
#include "bdc.h"

__typeof__(MemberFnPtr[6]) g_uiBattleRecordPhaseTable = {
    { .pfn = (void *)UiBattleRecordLoadPhase }, { .pfn = (void *)UiBattleRecordFinishPhase },
    { .pfn = (void *)UiBattleRecordMenuPhase }, { .pfn = (void *)UiBattleRecordOpenPhase },
    { .pfn = (void *)UiBattleRecordModeListPhase }, { .pfn = (void *)UiBattleRecordTotalListPhase },
};
