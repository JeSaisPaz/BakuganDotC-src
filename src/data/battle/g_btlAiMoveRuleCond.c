// bdc 0x08a80380 g_btlAiMoveRuleCond
#include "bdc.h"

__typeof__(MemberFnPtr) g_btlAiMoveRuleCond = { .pfn = (void *)BtlAiEvalConditionXZ };
