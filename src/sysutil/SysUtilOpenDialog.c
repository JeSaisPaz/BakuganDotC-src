// bdc 0x089cc420 SysUtilOpenDialog
#include "bdc.h"

/* Creates the system-utility dialog handler of kind `kind` and stores it in the manager's
   dialog table (`g_sysUtilMng``->dialogs[kind]`): kind 0
   builds the 0x14-byte handler `SysUtilMsgDialogHandlerCtor` (0x2c8-byte parameter block),
   kind 1 the 0x20-byte savedata handler `SysUtilSavedataHandlerCtor` (0x76c-byte
   `SceUtilitySavedataParam` block, `ULES01466`/`BAKUGAN2`/`PLAYDATA.BIN`); kinds 2-3 and negative
   kinds create nothing. For every `kind < 4` it then requests a software-driven suspend cycle
   from the power manager (`CorePowerRequestSuspend`: sets `manualSuspend`, so the next
   `CorePowerUpdate` releases the volatile memory region); `kind >= 4` returns without doing
   anything. Both handlers are
   allocated from the low end of the heap (allocation policy saved and restored under
   `MemLock`). `cell` is unused. */

void SysUtilOpenDialog(u32 *cell, s32 kind)
{
  bool fromLow;
  SysUtilHandler *handler;
  SysUtilHandler *mem;

  if (kind >= 4) {
    return;
  }
  handler = NULL;
  if (kind == 0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(0x14, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      SysUtilMsgDialogHandlerCtor(mem);
      handler = mem;
    }
  }
  else if (kind == 1) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(0x20, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mem != NULL) {
      SysUtilSavedataHandlerCtor((SysUtilSavedataHandler *)mem);
      handler = mem;
    }
  }
  if (handler != NULL) {
    g_sysUtilMng->dialogs[kind] = handler;
  }
  CorePowerRequestSuspend(CorePowerGet());
}
