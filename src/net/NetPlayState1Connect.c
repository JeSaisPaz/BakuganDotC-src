// bdc 0x0881bff8 NetPlayState1Connect
#include "bdc.h"

/* Handler of NetPlay state 1 (`state`), entry 1 of the state table at `0x08a50970` run by
   `NetPlayUpdate`: starts the ad-hoc session through `substate`:
   0 creates the ad-hoc layer (`NetAdhocCreate`), shows `NetStatusSetMessage``(1, isHost ? 1 : 2)`
     and, if the manager exists, sets the role (`NetAdhocSetHost`) and moves to 1;
   1 requests the ad-hoc control connection (`NetAdhocRequestConnect``(mgr, 1, 0)`), 2 once accepted;
   2 polls it (`NetAdhocPollConnect`): connected moves to 3; failed with no pending net error
     (`NetErrorHasPending`) and a clear `NetInviteGetFlag0c` goes to state 8 and sets save-profile
     flag `0x80`; any result other than "still connecting" hides the status message;
   3 clears `peerCount`; with a manager it reads the local packet header (`NetCharaGetSentHeader`,
     returning to the common tail if that fails) and, for the host, copies its host info and MAC into
     `peers[0]` and sets `peerCount` to 1; then moves to state 2 (host) or 4 (guest, clearing
     `guestJoinWord`) and clears `resetOnConnect`.
   Every call ends by checking `NetAdhocHasManager`: without the ad-hoc manager it switches to
   state 8 and sets save-profile flag `0x80`. */

void NetPlayState1Connect(NetPlay *self)
{
    NetCharaPacketHeader hdr;
    char name[64];
    NetPlayPeer *peer;
    s32 result;
    s32 i;

    switch (self->substate) {
    case 0:
        NetAdhocCreate();
        if (self->isHost) {
            NetStatusSetMessage(1, 1);
        } else {
            NetStatusSetMessage(1, 2);
        }
        if (NetAdhocHasManager()) {
            NetAdhocSetHost(NetAdhocGetManager(), self->isHost);
            self->substate = 1;
        }
        break;
    case 1:
        if (NetAdhocHasManager() && NetAdhocRequestConnect(NetAdhocGetManager(), 1, 0)) {
            self->substate = 2;
        }
        break;
    case 2:
        if (NetAdhocHasManager()) {
            result = NetAdhocPollConnect(NetAdhocGetManager());
            if (result > 0) {
                self->substate = 3;
            } else if (result < 0 && !NetErrorHasPending(NetErrorGetManager()) &&
                       !NetInviteGetFlag0c(NetErrorGetManager())) {
                self->state = 8;
                self->substate = 0;
                if (SaveHasProfile()) {
                    SaveProfileSetFlags(SaveGetProfile(), 0x80);
                }
            }
            if (result != 0) {
                NetStatusSetMessage(0, 0);
            }
        }
        break;
    case 3:
        self->peerCount = 0;
        if (NetAdhocHasManager()) {
            if (!NetCharaGetSentHeader(&hdr, 0)) {
                break;
            }
            if (self->isHost) {
                peer = &self->peers[0];
                memcpy(peer->mac, hdr.mac, 6);
                for (i = 0; i < 24; i++) {
                    peer->info[i] = hdr.hostInfo[i];
                }
                peer->info[24] = hdr.hostInfoEnd;
                memset(name, 0, sizeof(name));
                strncpy(name, (char *)peer->info, 25);
                self->peerCount = 1;
            }
        }
        if (self->isHost) {
            self->state = 2;
            self->substate = 0;
        } else {
            self->state = 4;
            self->substate = 0;
            self->guestJoinWord = 0;
        }
        self->resetOnConnect = 0;
        break;
    }
    if (!NetAdhocHasManager()) {
        self->state = 8;
        self->substate = 0;
        if (SaveHasProfile()) {
            SaveProfileSetFlags(SaveGetProfile(), 0x80);
        }
    }
}
