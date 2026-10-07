// bdc 0x089d4c78 NetAdhocctlIsDisconnected
#include "bdc.h"

/* Returns 1 when `sceNetAdhocctlGetState` reports state 0, or fails with `0x80410b08` (adhocctl not
   initialised). */

bool NetAdhocctlIsDisconnected(NetAdhocConn *self)

{
  int result;
  bool disconnected;
  int state;
  
  disconnected = false;
  state = 0;
  result = sceNetAdhocctlGetState(&state);
  if (result == 0) {
    if (state == 0) {
      disconnected = true;
    }
  }
  else if (result == -0x7fbef4f8) {
    disconnected = true;
  }
  return disconnected;
}

