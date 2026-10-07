// bdc 0x089cc568 SysUtilCloseDialog
#include "bdc.h"

/* Destroys the dialog handler of kind `kind` (virtual destructor, flag 3), clears its slot in the
   manager's table and releases the suspend block with `CorePowerRequestResume`. Counterpart of
   `SysUtilOpenDialog`. */

void SysUtilCloseDialog(u32 *cell, s32 kind)
{
  SysUtilHandler **slot;
  SysUtilHandler *h;
  CorePower *power;

  if (kind < 4) {
    h = g_sysUtilMng->dialogs[kind];
    if (h != NULL) {
      ((void (*)(void *, int))h->vtbl[1].fn)((u8 *)h + h->vtbl[1].delta, 3);
      slot = &g_sysUtilMng->dialogs[kind];
      *slot = NULL;
    }
    power = CorePowerGet();
    CorePowerRequestResume(power);
  }
}
