// bdc 0x08af5a14 g_msgDialogHandlerVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_msgDialogHandlerVtbl = {
    {0}, { .fn = (void *)SysUtilMsgDialogHandlerDtor },
    { .fn = (void *)SysUtilMsgDialogHandlerRequest },
    { .fn = (void *)SysUtilMsgDialogHandlerAbort },
    { .fn = (void *)SysUtilMsgDialogHandlerService },
    { .fn = (void *)SysUtilMsgDialogHandlerRefreshState },
    { .fn = (void *)SysUtilMsgDialogHandlerUpdate },
};
