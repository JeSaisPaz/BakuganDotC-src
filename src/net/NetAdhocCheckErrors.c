// bdc 0x089d53d4 NetAdhocCheckErrors
#include "bdc.h"

/* Error check at the start of `NetAdhocUpdate`: with the WLAN switch on it withdraws the "WLAN
   off" error `0x80410b03`; with it off it posts that error when the connection is active (ctlState
   4/6 or phase 3/4). Then runs `NetErrorUpdate`; on a pending error (`NetAdhocHasError`) in
   ctlState 4 or 6 it requests the stop phase 5. Returns 1 when an error is pending or the WLAN-off
   error was posted for an active connection, else 0. */

bool NetAdhocCheckErrors(NetAdhocConn *self)
{
  bool error = false;
  s32 state;

  if (sceWlanGetSwitchState() != 0) {
    NetErrorRemove(NetErrorGetManager(), (s32)0x80410b03);
  } else {
    state = self->ctlState;
    if (state == 4 || state == 6) {
      error = true;
    }
    if (self->phase >= 3 && self->phase < 5) {
      error = true;
    }
    if (error) {
      NetAdhocPostError(self, (s32)0x80410b03);
    }
  }
  NetErrorUpdate(NetErrorGetManager());
  if (NetAdhocHasError()) {
    error = true;
    state = self->ctlState;
    if (state == 4 || state == 6) {
      NetAdhocSetPhase(self, 5);
    }
  }
  return error;
}
