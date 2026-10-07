// bdc 0x0881c4a8 NetPlayState3HostLaunch
#include "bdc.h"

/* Handler of NetPlay state 3 (`+4`), entry 3 of the state table at `0x08a50970` run by
   `NetPlayUpdate` (host side): substate 0 waits until no remote peer's net character exists any
   more and shows status message 0; 1..4 stop the lobby connection (`NetAdhocBeginStop`), wait for
   it, set the own MAC as host MAC, reopen the ad-hoc link in game mode
   (`NetAdhocRequestConnect``(mgr, 2, 1)`) and wait for it; substate 5 moves to state 7 (client
   MAC = peer 1) once every peer is present again, or to state 8 (profile flag `0x80`) when the link
   dropped. */

void NetPlayState3HostLaunch(NetPlay *self)
{
    bool ok;
    s32 i;
    u8 *mac;

    switch ((u32)self->substate) {
    case 0:
        /* wait until every remote peer's character is gone */
        ok = true;
        for (i = 1; i < self->peerCount; i++) {
            if (NetCharaFindByMac(self->peers[i].mac) != NULL) {
                ok = false;
                break;
            }
        }
        if (ok) {
            self->substate = 1;
            NetStatusSetMessage(1, 0);
        }
        break;
    case 1:
        if (NetAdhocBeginStop(NetAdhocGetManager())) {
            self->substate = 2;
        }
        break;
    case 2:
        if (NetAdhocIsDisconnected(NetAdhocGetManager())) {
            self->substate = 3;
            NetAdhocGetManager();
            mac = NetAdhocGetOwnMac();
            if (mac != NULL) {
                NetAdhocSetHostMac(NetAdhocGetManager(), mac);
            }
            NetCharaMgrRemoveRemotes();
        }
        break;
    case 3:
        if (NetAdhocRequestConnect(NetAdhocGetManager(), 2, 1)) {
            self->substate = 4;
        }
        break;
    case 4:
        if (NetAdhocPollConnect(NetAdhocGetManager()) != 0) {
            self->substate = 5;
        }
        break;
    case 5:
        /* wait until every remote peer's character is back */
        ok = true;
        for (i = 1; i < self->peerCount; i++) {
            if (NetCharaFindByMac(self->peers[i].mac) == NULL) {
                ok = false;
                break;
            }
        }
        if (ok) {
            if (NetAdhocHasManager()) {
                NetAdhocSetClientMac(NetAdhocGetManager(), self->peers[1].mac);
            }
            self->state = 7;
            self->substate = 0;
            NetStatusSetMessage(0, 0);
        } else if (NetAdhocIsDisconnected(NetAdhocGetManager())) {
            self->state = 8;
            self->substate = 0;
            if (SaveHasProfile()) {
                SaveProfileSetFlags(SaveGetProfile(), 0x80);
            }
        }
        break;
    default:
        break;
    }
}
