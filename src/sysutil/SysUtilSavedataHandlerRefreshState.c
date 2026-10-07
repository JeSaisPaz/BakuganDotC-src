// bdc 0x089cc768 SysUtilSavedataHandlerRefreshState
#include "bdc.h"

/* Savedata handler virtual: stores `sceUtilitySavedataGetStatus()` in the handler's dialog state
   word (`+4`, read by `SysUtilHandlerGetState`). */

void SysUtilSavedataHandlerRefreshState(SysUtilSavedataHandler *self)

{
  int status;

  status = sceUtilitySavedataGetStatus();
  (self->base).state = status;
  return;
}
