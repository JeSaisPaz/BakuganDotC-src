// bdc 0x089cc0ac SysUtilPoll
#include "bdc.h"

/* Per-frame activation step from `BootMainThread`. Skipped unless the handler list exists and
   has entries and the power manager permits running (`CorePowerIsInitialized`,
   `CorePowerCanRunFrame`). It then runs `CorePrioListMerge`, walks the live handlers in
   priority order and for the first enabled one calls virtual slot `+0x24` (a "service/start"
   request check), stores the handler's id (`+0x8`) in `*cell` and stops. Returns early, leaving
   `*cell` untouched, at the first live node with a NULL payload. */

void SysUtilPoll(u32 *cell)

{
  CorePower *power;
  CorePrioNode *node;
  SysUtilHandler *self;

  if (g_sysUtilMng->handlers == (CorePrioList *)0x0) {
    return;
  }
  if (!CorePrioListHasEntries(g_sysUtilMng->handlers)) {
    return;
  }
  if (CorePowerIsInitialized() == 0) {
    return;
  }
  power = CorePowerGet();
  if (CorePowerCanRunFrame(power) == 0) {
    return;
  }
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
        ((void (*)(void *))self->vtbl[4].fn)((char *)self + self->vtbl[4].delta);
        *cell = self->id;
        return;
      }
    }
  }
}
