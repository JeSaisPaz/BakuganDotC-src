// bdc 0x089d4bd4 NetAdhocctlDisconnectStep
#include "bdc.h"

/* If the adhocctl state is 1 or 3 (state 2 and anything else is left alone), clears the events
   and calls `sceNetAdhocctlDisconnect`; returns 1 when the disconnect succeeds, 0 otherwise;
   errors other than `0x80410b10` are posted via NetAdhocPostError. */

bool NetAdhocctlDisconnectStep(NetAdhocConn *self)
{
  int err;
  bool issued;
  int state;

  issued = false;
  state = 0;
  err = sceNetAdhocctlGetState(&state);
  if (err == 0) {
    if (state < 2) {
      if (state < 1) {
        return false;
      }
    }
    else if (state != 3) {
      return false;
    }
    NetAdhocClearEvents(self);
    err = sceNetAdhocctlDisconnect();
    if (err == 0) {
      issued = true;
    }
    else if (err != (int)0x80410b10) {
      NetAdhocPostError(self, err);
    }
  }
  return issued;
}
