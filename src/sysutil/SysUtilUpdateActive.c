// bdc 0x089cc1c0 SysUtilUpdateActive
#include "bdc.h"

/* Frame-end update of the active system utility, called from `GfxDisplayEndFrame` between the GE
   sync and the buffer swap. Same guards and walk as `SysUtilPoll` (`CorePrioListMerge`, first
   enabled live handler), but calls virtual slot `+0x34` (the handler's per-frame utility update —
   where the PSP utility dialog is advanced and drawn) instead of `+0x24`. */

void SysUtilUpdateActive(void)

{
  int ok;
  CorePower *power;
  CorePrioNode *node;
  SysUtilHandler *self;

  ok = CorePowerIsInitialized();
  if (ok != 0) {
    power = CorePowerGet();
    ok = CorePowerCanRunFrame(power);
    if (((ok != 0) && (g_sysUtilMng->handlers != (CorePrioList *)0x0)) &&
       (CorePrioListHasEntries(g_sysUtilMng->handlers))) {
      CorePrioListMerge(g_sysUtilMng->handlers);
      node = CorePrioListHead(g_sysUtilMng->handlers);
      for (node = CorePrioNodeGetNext(node); node != (CorePrioNode *)0x0;
          node = CorePrioNodeGetNext(node)) {
        if (!CorePrioNodeIsRemoved(node)) {
          self = CorePrioNodeGetData(node);
          if (self == (SysUtilHandler *)0x0) {
            return;
          }
          if (SysUtilHandlerIsEnabled(self)) {
            ((void (*)(void *))self->vtbl[6].fn)((char *)self + self->vtbl[6].delta);
            return;
          }
        }
      }
    }
  }
  return;
}
