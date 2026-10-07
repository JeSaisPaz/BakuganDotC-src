// bdc 0x089c6dec SndManagerStateLoadModules
#include "bdc.h"

/* State 1 of the sound manager (`SndManagerStep`): once the module manager singleton exists
   (`CoreHasModuleMgr`, `g_coreModuleMgr != 0`) it asks it to load modules 0, 1 and 2 (the ids
   come from the table `g_sndModuleIds` = {0, 1, 2}; `CoreModuleRequestLoad(modMgr, id)` marks the
   module slot 'load requested' and wakes the loader thread). When all three requests were accepted
   it moves the manager to state 2. */

void SndManagerStateLoadModules(SndManager *mgr)
{
  bool ok = true;
  int i;

  if (CoreHasModuleMgr()) {
    for (i = 0; i < 3; i++) {
      if (CoreModuleRequestLoad(CoreGetModuleMgr(), g_sndModuleIds[i]) == 0) {
        ok = false;
      }
    }
    if (ok) {
      mgr->state = 2;
    }
  }
}
