// bdc 0x08a99a50 g_btlAppearDemoUpdateTable
#include "bdc.h"

__typeof__(MemberFnPtr[2]) g_btlAppearDemoUpdateTable = { { .pfn = (void *)BtlAppearDemoStateLoad }, { .pfn = (void *)BtlAppearDemoStatePlay } };
