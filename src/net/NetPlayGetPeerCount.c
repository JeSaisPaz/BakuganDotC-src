// bdc 0x0881b238 NetPlayGetPeerCount
#include "bdc.h"

/* Returns the number of entries in the `NetPlay` peer table (at most 4). */
s32 NetPlayGetPeerCount(NetPlay *self)
{
    return self->peerCount;
}
