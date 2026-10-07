// bdc 0x08af52f4 g_sysUtilSavedataVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_sysUtilSavedataVtbl = {
    {0}, { .fn = (void *)SysUtilSavedataHandlerDtor },
    { .fn = (void *)SysUtilSavedataHandlerRequest }, { .fn = (void *)SysUtilSavedataHandlerAbort },
    { .fn = (void *)SysUtilSavedataHandlerService },
    { .fn = (void *)SysUtilSavedataHandlerRefreshState },
    { .fn = (void *)SysUtilSavedataHandlerUpdate },
};
