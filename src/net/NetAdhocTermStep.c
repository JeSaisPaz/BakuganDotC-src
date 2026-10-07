// bdc 0x089d45dc NetAdhocTermStep
#include "bdc.h"

/* Connection state 5 of `NetAdhocUpdate`, stepped by `step`: flushes/closes the PDP object until
   it is gone (`NetAdhocPdpGone`, `NetAdhocFlushPdp`), disconnects adhocctl
   (`NetAdhocctlDisconnectStep`, `NetAdhocctlIsDisconnected`), removes the handler
   (`sceNetAdhocctlDelHandler`, skipped when `handlerId` < 0; `handlerId` becomes -1), then
   `sceNetAdhocctlTerm`, `sceNetAdhocTerm`, `sceNetTerm`. Returns 0 while working, 1 once `step` is
   past the last sub-step, or the nonzero sce error of the current sub-step. */

s32 NetAdhocTermStep(NetAdhocConn *self)
{
  s32 ret = 0;

  switch (self->step) {
  case 0:
    if (!NetAdhocPdpGone(self)) {
      NetAdhocFlushPdp(self);
    } else {
      self->step++;
    }
    break;
  case 1:
    if (NetAdhocctlIsDisconnected(self)) {
      self->step++;
    } else if (NetAdhocctlDisconnectStep(self)) {
      self->step++;
    }
    break;
  case 2:
    if (NetAdhocctlIsDisconnected(self)) {
      self->step++;
    }
    break;
  case 3:
    if (self->handlerId >= 0) {
      ret = sceNetAdhocctlDelHandler(self->handlerId);
    }
    if (ret == 0) {
      self->handlerId = -1;
      self->step++;
    }
    break;
  case 4:
    ret = sceNetAdhocctlTerm();
    if (ret == 0) {
      self->step++;
    }
    break;
  case 5:
    ret = sceNetAdhocTerm();
    if (ret == 0) {
      self->step++;
    }
    break;
  case 6:
    ret = sceNetTerm();
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
