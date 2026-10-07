// bdc 0x08a99a40 g_btlDemoDrawTable
#include "bdc.h"

__typeof__(MemberFnPtr[2]) g_btlDemoDrawTable = { { .pfn = (void *)BtlDemoDrawFade }, { .pfn = (void *)BtlDemoDrawPlay } };
