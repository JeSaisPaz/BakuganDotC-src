// bdc 0x0881c8fc NetPlayState5JoinWait
#include "bdc.h"

/* Handler of NetPlay state 5 (`+4`), entry 5 of the state table at `0x08a50970` run by
   `NetPlayUpdate` (client side): waits while connected to the chosen host. If the local header
   is available and the host (`selectedHost.mac`, `+0xad`) has no net character any more, it goes
   back to the scan state 4 (`NetPlaySetState``(netPlay, 4)`) and zeroes the peer count. If the
   link is idle it clears the peer list and aborts unless the ad-hoc phase is 3; with no ad-hoc
   manager it aborts too. Aborting sets state 8 / substate 0 and the profile flag 0x80. */

void NetPlayState5JoinWait(NetPlay *self)
{
    int abort = 0;
    u32 i;
    NetCharaPacketHeader hdr;

    if (!NetAdhocHasManager()) {
        abort = 1;
    } else if (NetAdhocIsDisconnected(NetAdhocGetManager())) {
        self->peerCount = 0;
        for (i = 0; i < 4; i++) {
            memset(&self->peers[i], 0, sizeof(NetPlayPeer));
        }
        if (NetAdhocGetPhase(NetAdhocGetManager()) != 3) {
            abort = 1;
        }
    } else if (NetCharaGetSentHeader(&hdr, 0) != 0 &&
               NetCharaFindByMac(self->selectedHost.mac) == NULL) {
        NetPlaySetState(self, 4);
        self->peerCount = 0;
        return;
    }

    if (abort) {
        self->state = 8;
        self->substate = 0;
        if (SaveHasProfile()) {
            SaveProfileSetFlags((SaveProfile *)SaveGetProfile(), 0x80);
        }
    }
}
