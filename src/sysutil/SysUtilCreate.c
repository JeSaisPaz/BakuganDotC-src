// bdc 0x089cbddc SysUtilCreate
#include "bdc.h"

/* Creates the system-utility manager `g_sysUtilMng`: a zeroed 0x18-byte block (low heap), then
   its handler list (0x14-byte `CorePrioList` built with `CorePrioListInit`(list, 8), stored at
   `+0x4`) and the 4-byte selection cell (`SysUtilCellInit`, stored at `+0x0`). Each allocation
   is made from the low end of the heap under `MemLock`, restoring the previous placement
   policy; a failed list or cell allocation stores NULL. */

void SysUtilCreate(void)

{
  bool wasLow;
  SysUtilMng *mng;
  CorePrioList *list;
  u32 *cell;

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mng = MemAlloc(sizeof(SysUtilMng), (char *)0x0, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  g_sysUtilMng = mng;
  memset(mng, 0, sizeof(SysUtilMng));

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  list = MemAlloc(sizeof(CorePrioList), (char *)0x0, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  if (list != (CorePrioList *)0x0) {
    CorePrioListInit(list, 8);
  }
  g_sysUtilMng->handlers = list;

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  cell = MemAlloc(sizeof(u32), (char *)0x0, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  if (cell != (u32 *)0x0) {
    SysUtilCellInit(cell);
  }
  g_sysUtilMng->cell = cell;
}
