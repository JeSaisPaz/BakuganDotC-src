// bdc 0x08a80358 g_btlAiTargetPickers
#include "bdc.h"

__typeof__(MemberFnPtr[5]) g_btlAiTargetPickers = {
    { .pfn = (void *)BtlAiPickTargetId }, { .pfn = (void *)BtlAiPickNearestClass84InRange },
    { .pfn = (void *)BtlAiPickNearestClass5COr7CInView },
    { .pfn = (void *)BtlAiPickNearestClass54InView },
    { .pfn = (void *)BtlAiPickNearestClass84InView },
};
