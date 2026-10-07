// bdc 0x089d32b0 NetPdpDeleteSockets
#include "bdc.h"

/* Deletes the two ad-hoc PDP sockets of the `CONetPDP` object (ids at `+0x4` and `+0x8`), setting
   each successfully deleted id to -1. Returns 1 when both are closed (or were never open), 0 if
   `sceNetAdhocPdpDelete` failed for either. */

bool NetPdpDeleteSockets(NetPdp *self)

{
  int ret;
  bool ok;
  
  ok = true;
  if (0 < self->recvId) {
    ret = sceNetAdhocPdpDelete(self->recvId,0);
    if (ret == 0) {
      self->recvId = -1;
    }
    else {
      ok = false;
    }
  }
  if (0 < self->sendId) {
    ret = sceNetAdhocPdpDelete(self->sendId,0);
    if (ret == 0) {
      self->sendId = -1;
    }
    else {
      ok = false;
    }
  }
  return ok;
}

