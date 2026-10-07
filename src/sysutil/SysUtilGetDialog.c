// bdc 0x089cc5f0 SysUtilGetDialog
#include "bdc.h"

/* Returns the dialog handler of kind `kind` from the manager's dialog table
   (`g_sysUtilMng``->dialogs`), or NULL for `kind >= 4`
   (negative kinds are not checked). `cell` is unused. See `SysUtilOpenDialog`. */

void *SysUtilGetDialog(u32 *cell, s32 kind)
{
  if (kind < 4) {
    return g_sysUtilMng->dialogs[kind];
  }
  return NULL;
}
