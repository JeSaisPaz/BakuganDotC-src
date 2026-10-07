// bdc 0x0881bb0c NetPlayStartHost
#include "bdc.h"

/* Starts hosting: when the `NetPlay` manager is idle (state 0) it enters state 1
   (connect) with isHost = 1 and local player index 0. Returns true when started.
   Used by `UiNetLobbyHostPhase`. */

bool NetPlayStartHost(NetPlay *self)
{
    bool started = self->state == 0;

    if (started) {
        self->state = 1;
        self->substate = 0;
        self->isHost = 1;
        NetSetLocalPlayerIndex(0);
    }
    return started;
}
