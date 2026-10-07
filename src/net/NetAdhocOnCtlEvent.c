// bdc 0x089d3c68 NetAdhocOnCtlEvent
#include "bdc.h"

/* Records an adhocctl event on the COPSPNet connection object: sets bit `event` in the event mask
   `+0xc`. Event 0 (error) stores the error code at `+0x8` and posts it to the net error manager
   (`NetErrorPush`) unless already listed; event 1 (connect) clears the disconnect bit 2, event 2
   (disconnect) clears the connect bit 1, event 4 (game mode) clears both. */

void NetAdhocOnCtlEvent(NetAdhocConn *self, s32 event, s32 error)

{
  self->eventMask = self->eventMask | 1 << (event & 0x1fU);
  if (event == 0) {
    self->lastError = error;
    if (NetErrorHasManager()) {
      if (!NetErrorContains(NetErrorGetManager(),error)) {
        NetErrorPush(NetErrorGetManager(),error);
        return;
      }
    }
  }
  else {
    if (event == 1) {
      self->eventMask = self->eventMask & 0xfffffffb;
      return;
    }
    if (event == 2) {
      self->eventMask = self->eventMask & 0xfffffffd;
      return;
    }
    if ((event != 3) && (event == 4)) {
      self->eventMask = self->eventMask & 0xfffffff9;
    }
  }
  return;
}

