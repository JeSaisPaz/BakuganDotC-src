// bdc 0x089cc790 SysUtilSavedataHandlerUpdate
#include "bdc.h"

/* Savedata handler virtual update: when the dialog is visible (virtual state getter at vtable
   `+0x2c` returns 2) calls `sceUtilitySavedataUpdate(n)` with `n` = 2 frames when the display runs
   at frame skip 1 (`g_gfxDisplay``->frameSkip`), else 1. Returns 1 if the update succeeded, 0
   otherwise. */

int SysUtilSavedataHandlerUpdate(SysUtilSavedataHandler *self)
{
  int result = 0;
  int n = 1;
  int skip = g_gfxDisplay->frameSkip;
  const VtblEntry *vt;

  if (skip > 0 && skip < 2) {
    n = 2;
  }
  vt = self->base.vtbl;
  if (((int (*)(void *))vt[5].fn)((u8 *)self + vt[5].delta) == 2) {
    if (sceUtilitySavedataUpdate(n) == 0) {
      result = 1;
    }
  }
  return result;
}
