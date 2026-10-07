// bdc 0x089c7194 SndManagerStateUnloadModules
#include "bdc.h"

/* State 7 of the sound manager (`SndManagerStep`, last shutdown step): polls the module manager with
   `CoreModuleIsUnloaded(modMgr, id)` for each of the three ids in `g_sndModuleIds` (no request is
   issued here); when all three are unloaded the manager returns to state 0 (idle), otherwise it stays
   in state 7 and polls again next step. */

void SndManagerStateUnloadModules(SndManager *mgr)

{
  bool allUnloaded;
  int i;

  allUnloaded = true;
  for (i = 0; i < 3; i++) {
    if (CoreModuleIsUnloaded(CoreGetModuleMgr(), g_sndModuleIds[i]) == 0) {
      allUnloaded = false;
    }
  }
  if (allUnloaded) {
    mgr->state = 0;
  }
  return;
}
