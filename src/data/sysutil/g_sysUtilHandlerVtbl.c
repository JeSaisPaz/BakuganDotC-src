// bdc 0x08af52bc g_sysUtilHandlerVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_sysUtilHandlerVtbl = {
    {0}, { .fn = (void *)SysUtilHandlerDtor }, { .fn = (void *)CxxPureVirtualCalled },
    { .fn = (void *)CxxPureVirtualCalled }, { .fn = (void *)CxxPureVirtualCalled },
    { .fn = (void *)CxxPureVirtualCalled }, { .fn = (void *)CxxPureVirtualCalled },
};
