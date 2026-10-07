// bdc 0x08a99888 g_btlDemoCamStateTable
#include "bdc.h"

__typeof__(MemberFnPtr[1]) g_btlDemoCamStateTable = { { .pfn = (void *)BtlDemoCamStateMain } };
