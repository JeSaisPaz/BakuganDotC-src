// bdc 0x089d5524 GfxMovieLoadModulesStep
#include "bdc.h"

/* One step of loading the three movie modules listed at `0x08aa2d3c` through the module manager:
   steps 0..2 load module `step` (`CoreModuleRequestLoad`), steps 3..5 start module `step-3`
   (`CoreModuleIsRunning`); the step advances only when the call succeeds. Returns the next step,
   -1 once all six are done (negative input passes through). */

s32 GfxMovieLoadModulesStep(s32 step)

{
  CoreModuleMgr *mgr;
  int ok;
  
  if (-1 < step) {
    if (step < 3) {
      mgr = CoreGetModuleMgr();
      ok = CoreModuleRequestLoad(mgr,g_movieModuleSlots[step]);
      if (ok != 0) {
        step = step + 1;
      }
    }
    else if (step < 6) {
      mgr = CoreGetModuleMgr();
      ok = CoreModuleIsRunning(mgr,g_movieModuleSlots[step - 3]);
      if (ok != 0) {
        step = step + 1;
      }
    }
    else {
      step = -1;
    }
  }
  return step;
}

