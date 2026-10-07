// bdc 0x0881c710 NetPlayState4ScanHosts
#include "bdc.h"

/* Handler of NetPlay state 4 (`+4`), entry 4 of the state table at `0x08a50970` run by
   `NetPlayUpdate`: while the ad-hoc link is up it scans the received peer headers
   (`NetCharaGetRecvHeader`, entries 1..15, stopping at the first missing one) and copies up to
   four hosts that advertise `lenFlags` bit 0x1000 (open game) into the peer table (host info +
   MAC), stores their count in `peerCount` and clears the unused records. When the link is idle it
   clears the table and stays unless the connection phase is not 3; then, or when the ad-hoc
   manager is missing, it switches to the abort state 8 and sets profile flag 0x80. */

void NetPlayState4ScanHosts(NetPlay *self)
{
    bool lost = false;

    if (!NetAdhocHasManager()) {
        lost = true;
    } else if (NetAdhocIsDisconnected(NetAdhocGetManager())) {
        u32 i;

        self->peerCount = 0;
        for (i = 0; i < 4; i++) {
            memset(&self->peers[i], 0, sizeof(NetPlayPeer));
        }
        if (NetAdhocGetPhase(NetAdhocGetManager()) != 3) {
            lost = true;
        }
    } else {
        NetCharaPacketHeader hdr;
        s32 count = 0;
        s32 index;
        u32 i;

        for (index = 1; index < 0x10; index++) {
            if (!NetCharaGetRecvHeader(&hdr, index)) {
                break;
            }
            if ((hdr.lenFlags & 0x1000) != 0) {
                NetPlayPeer *peer = &self->peers[count];
                int j;

                memcpy(peer->mac, hdr.mac, 6);
                /* Inline byte copy of the 0x19-byte host info (header +0x08..+0x20). */
                for (j = 0; j < 0x18; j++) {
                    peer->info[j] = hdr.hostInfo[j];
                }
                peer->info[0x18] = hdr.hostInfoEnd;
                count++;
                if (count >= 4) {
                    break;
                }
            }
        }
        self->peerCount = count;
        for (i = (u32)count; i < 4; i++) {
            memset(&self->peers[i], 0, sizeof(NetPlayPeer));
        }
    }

    if (lost) {
        self->state = 8;
        self->substate = 0;
        if (SaveHasProfile()) {
            SaveProfileSetFlags(SaveGetProfile(), 0x80);
        }
    }
}
