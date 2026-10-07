// bdc 0x089cc2c0 SysUtilIsBusy
#include "bdc.h"

/* Returns 1 if some registered handler has a dialog in progress: walks the handler list and tests
   `SysUtilHandlerGetState` for 1 (init), 2 (visible) or 4 (finished); state 0 and 3 do not count.
   The argument (the cell) is ignored. */

bool SysUtilIsBusy(u32 *cell)
{
  bool busy = false;
  CorePrioNode *node;
  SysUtilHandler *self;
  s32 state;

  if (g_sysUtilMng->handlers != NULL && CorePrioListHasEntries(g_sysUtilMng->handlers)) {
    node = CorePrioNodeGetNext(CorePrioListHead(g_sysUtilMng->handlers));
    while (node != NULL) {
      if (!CorePrioNodeIsRemoved(node)) {
        self = CorePrioNodeGetData(node);
        if (self == NULL) {
          return busy;
        }
        state = SysUtilHandlerGetState(self);
        if (state == 1 || state == 2 || state == 4) {
          busy = true;
        }
        if (busy) {
          return true;
        }
      }
      node = CorePrioNodeGetNext(node);
    }
  }
  return busy;
}
