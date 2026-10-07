// bdc 0x089cc730 SysUtilSavedataHandlerAbort
#include "bdc.h"

/* Abort virtual of the savedata system-utility handler (vtable `0x08af52f4` slot `+0x1c`): does
   nothing and returns 0, so a running savedata dialog cannot be aborted (unlike
   `SysUtilMsgDialogHandlerAbort`, which returns 1). */

int SysUtilSavedataHandlerAbort(SysUtilSavedataHandler *self)
{
    (void)self;
    return 0;
}
