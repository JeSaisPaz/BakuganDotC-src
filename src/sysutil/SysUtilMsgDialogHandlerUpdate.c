// bdc 0x08a0137c SysUtilMsgDialogHandlerUpdate
#include "bdc.h"

/* Update virtual of the PSP message-dialog handler (error dialog, vtable `0x08af5a14`): when the
   dialog is visible (state getter at vtable `+0x2c` returns 2) calls `sceUtilityMsgDialogUpdate(n)`
   with `n` = 2 at frame skip 1, else 1. Returns 1 on success. */

int SysUtilMsgDialogHandlerUpdate(SysUtilHandler *self)
{
  int result = 0;
  int n = 1;
  int skip = g_gfxDisplay->frameSkip;
  const VtblEntry *vt;

  if (skip > 0 && skip < 2) {
    n = 2;
  }
  vt = self->vtbl;
  if (((int (*)(void *))vt[5].fn)((u8 *)self + vt[5].delta) == 2) {
    if (sceUtilityMsgDialogUpdate(n) == 0) {
      result = 1;
    }
  }
  return result;
}
