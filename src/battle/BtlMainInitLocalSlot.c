// bdc 0x08850b08 BtlMainInitLocalSlot
#include "bdc.h"

/* Refreshes the profile flag cache (`SaveRefreshProfileFlag0`) and sets the local player slot
   localSlot: 0, or the NetPlay local slot (`NetPlayGetLocalSlot`) when the NetPlay manager
   exists (`NetPlayHasManager`). */

void BtlMainInitLocalSlot(BtlMain *self)
{
    SaveRefreshProfileFlag0();
    self->localSlot = 0;
    if (NetPlayHasManager()) {
        self->localSlot = NetPlayGetLocalSlot(NetPlayGetManager());
    }
}
