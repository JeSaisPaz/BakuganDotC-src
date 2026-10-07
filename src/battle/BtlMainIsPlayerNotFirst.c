// bdc 0x0884d8cc BtlMainIsPlayerNotFirst
#include "bdc.h"

/* True when the local player's placing (`BtlMainGetPlayerPlacing` for slot `localSlot`) is not
   0 (first); the "no ranking" placing 4 also counts as not first. */
bool BtlMainIsPlayerNotFirst(BtlMain *self)
{
    return BtlMainGetPlayerPlacing(self, self->localSlot) != 0;
}
