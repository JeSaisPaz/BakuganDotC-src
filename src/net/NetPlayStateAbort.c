// bdc 0x0881cda0 NetPlayStateAbort
#include "bdc.h"

/* State-8 handler of the NetPlay state machine (last entry of the handler table at `0x08a50970`,
   run by `NetPlayUpdate` when the abort is requested or a lobby step fails): steps the net-adhoc
   layer down through substates held in `+8`. 0: `NetAdhocBeginStop` -> 1:
   `NetAdhocIsDisconnected` -> 2: `NetAdhocStopLink` -> 3: `NetAdhocIsStopped` (then
   `NetAdhocRequestThreadExit` sets `g_netAdhocThreadExit`) -> 4: waits until
   `NetAdhocHasManager` turns false, then sets the `finished` byte `+0xd`, which makes
   `NetPlayUpdate` call `NetPlayShutdown`. */

void NetPlayStateAbort(NetPlay *self)
{
  switch (self->substate) {
  case 0:
    if (NetAdhocBeginStop(NetAdhocGetManager())) {
      self->substate = 1;
    }
    break;
  case 1:
    if (NetAdhocIsDisconnected(NetAdhocGetManager())) {
      self->substate = 2;
    }
    break;
  case 2:
    if (NetAdhocStopLink(NetAdhocGetManager())) {
      self->substate = 3;
    }
    break;
  case 3:
    if (NetAdhocIsStopped(NetAdhocGetManager())) {
      NetAdhocGetManager();
      NetAdhocRequestThreadExit();
      self->substate = 4;
    }
    break;
  case 4:
    if (!NetAdhocHasManager()) {
      self->finished = 1;
    }
    break;
  }
}
