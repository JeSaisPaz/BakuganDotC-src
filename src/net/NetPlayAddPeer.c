// bdc 0x0881b33c NetPlayAddPeer
#include "bdc.h"

/* Appends the 0x1f-byte peer record `rec` to the `NetPlay` peer table (`+0x18`, count
   `+0x14`) if there is room (fewer than 4). Returns 1 when added. Used by
   `NetCharaHandshakeStep`. */

s32 NetPlayAddPeer(NetPlay *self, const u8 *rec)
{
    u8 *dst;
    int i;

    if (rec == NULL || (u32)self->peerCount >= 4) {
        return 0;
    }
    dst = (u8 *)&self->peers[self->peerCount];
    self->peerCount++;
    for (i = 0; i < (int)sizeof(NetPlayPeer); i++) {
        dst[i] = rec[i];
    }
    return 1;
}
