// bdc 0x089c70e8 SndManagerStateStopModules
#include "bdc.h"

/* State 6 of the sound manager (`SndManagerStep`, shutdown): while boot thread slot 9 is still
   running (`BootIsThreadRunning`) it nudges it (`BootWakeupThread(9)`) and waits; once it has
   ended it asks the module manager to stop modules 2, 1, 0 (reverse order, ids from `g_sndModuleIds`
   downwards, `CoreModuleRequestUnload`) and, when all three requests were accepted, moves to
   state 7 (`SndManagerStateUnloadModules`). */

void SndManagerStateStopModules(SndManager *mgr)
{
    bool ok = true;
    s32 i;

    if (BootIsThreadRunning(9) != 0) {
        BootWakeupThread(9);
        return;
    }
    for (i = 2; i >= 0; i--) {
        if (CoreModuleRequestUnload(CoreGetModuleMgr(), g_sndModuleIds[i]) == 0) {
            ok = false;
        }
    }
    if (ok) {
        mgr->state = 7;
    }
}
