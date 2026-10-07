// bdc 0x0881c2b4 NetPlayState2Lobby
#include "bdc.h"

/* Handler of NetPlay state 2 (`+4`), entry 2 of the state table at `0x08a50970` run by
   `NetPlayUpdate`: while the ad-hoc link is up (`NetAdhocIsDisconnected` == 0) it keeps the
   peer list current, removing every peer from index 1 on whose MAC `NetCharaFindByMac` no longer
   finds (later records shift down, `peerCount` drops), then publishes the count in net character
   0's outgoing header (`peerCount`, `lobbyReserved` cleared). When the link is idle it clears the
   table and stays unless the connection phase is not 3; then, or when the ad-hoc manager is
   missing, it switches to the abort state 8 and sets profile flag 0x80. */

void NetPlayState2Lobby(NetPlay *self)
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
        NetChara *chara;
        s32 i;

        NetAdhocGetManager(); /* result unused */
        NetAdhocGetOwnMac();  /* result unused */

        for (i = 1; i < self->peerCount;) {
            if (NetCharaFindByMac(self->peers[i].mac) == NULL) {
                s32 j;

                for (j = i; j < self->peerCount - 1; j++) {
                    self->peers[j] = self->peers[j + 1];
                }
                self->peerCount = self->peerCount - 1;
            } else {
                i++;
            }
        }

        chara = NetCharaGetByIndex(0);
        if (chara != NULL) {
            chara->outHdr.lobbyReserved[0] = 0;
            chara->outHdr.peerCount = (u8)self->peerCount;
            chara->outHdr.lobbyReserved[1] = 0;
            chara->outHdr.lobbyReserved[2] = 0;
            chara->outHdr.lobbyReserved[3] = 0;
            chara->outHdr.lobbyReserved[4] = 0;
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
