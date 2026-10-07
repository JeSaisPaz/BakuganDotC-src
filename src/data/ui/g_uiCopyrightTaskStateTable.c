// bdc 0x08a33a50 g_uiCopyrightTaskStateTable
#include "bdc.h"

__typeof__(MemberFnPtr[2]) g_uiCopyrightTaskStateTable = { { .pfn = (void *)UiCopyrightTaskStateShow }, { .pfn = (void *)UiCopyrightTaskStateEnd } };
