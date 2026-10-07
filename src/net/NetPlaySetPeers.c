// bdc 0x0881b4b0 NetPlaySetPeers
#include "bdc.h"

/* Replaces the `NetPlay` peer table with the four 0x1f-byte records at `recs` (when
   non-NULL) and sets the peer count to `count`. Used by `NetCharaHandshakeStep` when the host
   broadcasts the member list. */

void NetPlaySetPeers(NetPlay *self, const u8 *recs, s32 count)
{
    if (recs != NULL) {
        int i;
        int j;

        /* Four unrolled byte copies of 0x1f bytes each (no memcpy call). */
        for (i = 0; i < 4; i++) {
            u8 *dst = self->peers[i].info;
            const u8 *src = recs + i * 0x1f;

            for (j = 0; j < 0x1f; j++) {
                dst[j] = src[j];
            }
        }
    }
    self->peerCount = count;
}
