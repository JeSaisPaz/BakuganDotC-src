// bdc 0x08a80350 g_btlAiReactionRuleCond
#include "bdc.h"

__typeof__(MemberFnPtr) g_btlAiReactionRuleCond = { .pfn = (void *)BtlAiEvalCondition };
