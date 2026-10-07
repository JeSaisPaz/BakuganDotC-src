// bdc 0x0881bb54 NetPlayStartJoin
#include "bdc.h"

/* Starts joining: when the `NetPlay` manager is idle (state 0) it enters state 1
   (connect) with isHost = 0 and local player index 1. Returns true when started.
   Used by `UiNetLobbyJoinPhase`. */

bool NetPlayStartJoin(NetPlay *self)
{
    bool started = self->state == 0;

    if (started) {
        self->state = 1;
        self->substate = 0;
        self->isHost = 0;
        NetSetLocalPlayerIndex(1);
    }
    return started;
}
