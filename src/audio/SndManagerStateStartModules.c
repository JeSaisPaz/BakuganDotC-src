// bdc 0x089c6e74 SndManagerStateStartModules
#include "bdc.h"

/* State 2 of the sound manager (`SndManagerStep`): polls the module manager with
   `CoreModuleIsRunning(modMgr, id)` for each of the three ids in `g_sndModuleIds` (no request is
   issued here); when all three are running it moves the manager to state 3
   (`SndManagerStateInitAudio`), otherwise it stays in state 2 and polls again next step. */

void SndManagerStateStartModules(SndManager *mgr)
{
  bool ok = true;
  int i;

  for (i = 0; i < 3; i++) {
    if (CoreModuleIsRunning(CoreGetModuleMgr(), g_sndModuleIds[i]) == 0) {
      ok = false;
    }
  }
  if (ok) {
    mgr->state = 3;
  }
}
