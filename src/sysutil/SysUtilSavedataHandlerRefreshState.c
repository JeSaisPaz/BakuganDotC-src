// bdc 0x089cc768 SysUtilSavedataHandlerRefreshState
#include "bdc.h"

/* Savedata handler virtual: stores `sceUtilitySavedataGetStatus()` in the handler's dialog state
   word (`+4`, read by `SysUtilHandlerGetState`) and returns it (the caller
   `SysUtilSavedataHandlerUpdate` compares it with 2). */

s32 SysUtilSavedataHandlerRefreshState(SysUtilSavedataHandler *self)

{
  int status;

  status = sceUtilitySavedataGetStatus();
  (self->base).state = status;
  return status;
}
