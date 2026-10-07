// bdc 0x089d41dc NetAdhocLoadModules
#include "bdc.h"

/* Connection state 1 of `NetAdhocUpdate`, stepped by `step`: step 0 requests loading of the
   network modules 4 and 5 (`CoreModuleRequestLoad`), step 1 waits until both run
   (`CoreModuleIsRunning`), step 2 reads the WLAN MAC into `g_netAdhoc``->ownMac`
   (`sceWlanGetEtherAddr`). Returns 0 while working (or when there is no module manager),
   1 when done (any other step), or the sce error of `sceWlanGetEtherAddr`. */

s32 NetAdhocLoadModules(NetAdhocConn *self)
{
  s32 ret = 0;

  if (!CoreHasModuleMgr()) {
    return ret;
  }
  switch (self->step) {
  case 0:
    if (CoreModuleRequestLoad(CoreGetModuleMgr(), 4) == 0) {
      break;
    }
    if (CoreModuleRequestLoad(CoreGetModuleMgr(), 5) == 0) {
      break;
    }
    self->step++;
    break;
  case 1:
    if (CoreModuleIsRunning(CoreGetModuleMgr(), 4) == 0) {
      break;
    }
    if (CoreModuleIsRunning(CoreGetModuleMgr(), 5) == 0) {
      break;
    }
    self->step++;
    break;
  case 2:
    ret = sceWlanGetEtherAddr(g_netAdhoc->ownMac);
    if (ret == 0) {
      self->step++;
    }
    break;
  default:
    ret = 1;
    break;
  }
  return ret;
}
