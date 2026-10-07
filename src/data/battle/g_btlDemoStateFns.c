// bdc 0x08a99a30 g_btlDemoStateFns
#include "bdc.h"

__typeof__(MemberFnPtr[2]) g_btlDemoStateFns = { { .pfn = (void *)BtlDemoStateLoad }, { .pfn = (void *)BtlDemoStatePlay } };
