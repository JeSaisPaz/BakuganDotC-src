// bdc 0x089d3f30 NetAdhocBeginStop
#include "bdc.h"

/* First step of the adhoc tear-down run by `NetPlayStateAbort`: if the connection object is in
   phase 0 (`NetAdhocPhaseIs`) it moves it to phase 5 with `NetAdhocSetPhase` and returns 1,
   otherwise returns 0 (the abort handler then retries next frame). */

bool NetAdhocBeginStop(NetAdhocConn *self)
{
  bool ok;

  ok = NetAdhocPhaseIs(self, 0);
  if (ok) {
    NetAdhocSetPhase(self, 5);
  }
  return ok;
}
