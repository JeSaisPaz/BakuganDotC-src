// bdc 0x08a64b9c g_btlHudPhaseFns
#include "bdc.h"

__typeof__(const MemberFnPtr[10]) g_btlHudPhaseFns = {
    { .pfn = (void *)BtlHudPhaseWaitStart }, { .pfn = (void *)BtlHudPhaseBuild },
    { .pfn = (void *)BtlHudPhaseMain }, { .pfn = (void *)BtlHudPhaseBattleEnd },
    { .pfn = (void *)BtlHudPhaseResultSelect }, { .pfn = (void *)BtlHudPhaseResultWin },
    { .pfn = (void *)BtlHudPhaseResultLose }, { .pfn = (void *)BtlHudPhaseResultDraw },
    { .pfn = (void *)BtlHudPhaseRoundEnd }, { .pfn = (void *)BtlHudPhaseResult },
};
