// bdc 0x089cc05c SysUtilCellDelete
#include "bdc.h"

/* Deleting-destructor thunk of the selection cell: frees `cell` under `MemLock` when bit 0 of
   `flags` is set. */

void SysUtilCellDelete(u32 *cell, u32 flags)

{
  if ((cell != (u32 *)0x0) && ((flags & 1) != 0)) {
    MemLock();
    MemFree(cell,(char *)0x0,0);
    MemUnlock();
  }
  return;
}

