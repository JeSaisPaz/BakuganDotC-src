// bdc 0x089d4300 NetAdhocUnloadModules
#include "bdc.h"

/* Connection state 2 of `NetAdhocUpdate`, stepped by `step`: step 0 requests unloading
   (`CoreModuleRequestUnload`) of whichever of the network modules 4 and 5 is running, step 1
   waits until both are unloaded (`CoreModuleIsUnloaded`). Returns 0 while working (or when
   there is no module manager), 1 when done (any other step), -1 when an unload request failed
   (the step is then not advanced). */

s32 NetAdhocUnloadModules(NetAdhocConn *self)
{
  s32 ret = 0;

  if (!CoreHasModuleMgr()) {
    return ret;
  }
  switch (self->step) {
  case 0:
    if (CoreModuleIsRunning(CoreGetModuleMgr(), 4) != 0 &&
        CoreModuleRequestUnload(CoreGetModuleMgr(), 4) == 0) {
      ret = -1;
    }
    if (CoreModuleIsRunning(CoreGetModuleMgr(), 5) != 0 &&
        CoreModuleRequestUnload(CoreGetModuleMgr(), 5) == 0) {
      ret = -1;
    }
    if (ret == 0) {
      self->step++;
    }
    break;
  case 1:
    if (CoreModuleIsUnloaded(CoreGetModuleMgr(), 4) == 0) {
      break;
    }
    if (CoreModuleIsUnloaded(CoreGetModuleMgr(), 5) == 0) {
      break;
    }
    self->step++;
    break;
  default:
    ret = 1;
    break;
  }
  return ret;
}
