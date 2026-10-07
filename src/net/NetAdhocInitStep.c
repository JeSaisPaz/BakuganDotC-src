// bdc 0x089d443c NetAdhocInitStep
#include "bdc.h"

/* Connection state 4 of `NetAdhocUpdate`, stepped by `step`: `sceNetInit(0x40000, 30, 0, 30,
   0)`, `sceNetAdhocInit`, `sceNetAdhocctlInit` with the product struct of `g_netAdhoc`, registers
   `NetAdhocctlHandler` (id stored in `handlerId`) and builds the group name
   (`NetAdhocBuildGroupName`), connects (`NetAdhocctlConnectStep`), waits for the connect event
   (`NetAdhocHasConnectedEvent`) and creates the PDP socket object (`NetAdhocCreatePdp`), then
   waits until it exists (`NetAdhocHasPdp`). Returns 0 while working (each sub-step advances
   `step` on success), 1 once `step` is past the last sub-step, or the nonzero sce / connect error
   of the current sub-step. */

s32 NetAdhocInitStep(NetAdhocConn *self)
{
  s32 ret = 0;

  switch (self->step) {
  case 0:
    ret = sceNetInit(0x40000, 30, 0, 30, 0);
    if (ret == 0) {
      self->step++;
    }
    break;
  case 1:
    ret = sceNetAdhocInit();
    if (ret == 0) {
      self->step++;
    }
    break;
  case 2:
    ret = sceNetAdhocctlInit(0x4c00, 30, &g_netAdhoc->product);
    if (ret == 0) {
      self->step++;
    }
    break;
  case 3:
    NetAdhocClearEvents(self);
    ret = sceNetAdhocctlAddHandler(NetAdhocctlHandler, NULL);
    if (ret == 0) {
      self->handlerId = ret;
      NetAdhocBuildGroupName(self);
      self->step++;
    }
    break;
  case 4:
    ret = NetAdhocctlConnectStep(self, self->groupName);
    if (ret > 0) {  /* the binary tests < 0 and then <= 0 */
      ret = 0;
      self->step++;
    }
    break;
  case 5:
    if (NetAdhocHasConnectedEvent(self)) {
      NetAdhocCreatePdp(self);
      self->step++;
    }
    break;
  case 6:
    if (NetAdhocHasPdp(self)) {
      self->step++;
    }
    break;
  default:
    ret = 1;
    break;
  }
  return ret;
}
