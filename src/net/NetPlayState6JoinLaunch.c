// bdc 0x0881ca2c NetPlayState6JoinLaunch
#include "bdc.h"

/* Handler of NetPlay state 6 (`+4`), entry 6 of the state table at `0x08a50970` run by
   `NetPlayUpdate` (client side, counterpart of `NetPlayState3HostLaunch`): substates 0..3 stop
   the lobby connection (`NetAdhocBeginStop`, status message 0 shown), wait for it, set the
   selected host's MAC as host MAC and the own MAC as client MAC, reopen the ad-hoc link in game mode
   (`NetAdhocRequestConnect``(mgr, 2, 2)`) and wait for it; substate 4 moves to state 7 once the
   host's character (peer 0, `+0x31`) exists. In substates 3..4 a dropped link moves to state 8 and
   sets profile flag `0x80`; whenever that flag is set the state becomes 8. */

void NetPlayState6JoinLaunch(NetPlay *self)
{
    u8 *host;
    u8 *mac;

    switch ((u32)self->substate) {
    case 0:
        if (NetAdhocBeginStop(NetAdhocGetManager())) {
            self->substate = 1;
            NetStatusSetMessage(1, 0);
        }
        break;
    case 1:
        if (NetAdhocIsDisconnected(NetAdhocGetManager())) {
            self->substate = 2;
            host = NetPlayGetSelectedHost(self);
            if (host != NULL) {
                NetAdhocSetHostMac(NetAdhocGetManager(), ((NetPlayPeer *)host)->mac);
            }
            NetAdhocGetManager();
            mac = NetAdhocGetOwnMac();
            if (mac != NULL) {
                NetAdhocSetClientMac(NetAdhocGetManager(), mac);
            }
            NetCharaMgrRemoveRemotes();
        }
        break;
    case 2:
        if (NetAdhocRequestConnect(NetAdhocGetManager(), 2, 2)) {
            self->substate = 3;
        }
        break;
    case 3:
        if (NetAdhocPollConnect(NetAdhocGetManager()) != 0) {
            self->substate = 4;
        }
        break;
    case 4:
        if (NetCharaFindByMac(self->peers[0].mac) != NULL) {
            self->state = 7;
            self->substate = 0;
            NetStatusSetMessage(0, 0);
        }
        break;
    default:
        break;
    }

    if (self->substate >= 3 && self->substate < 5) {
        if (NetAdhocIsDisconnected(NetAdhocGetManager())) {
            self->state = 8;
            self->substate = 0;
            if (SaveHasProfile()) {
                SaveProfileSetFlags(SaveGetProfile(), 0x80);
            }
        }
    }

    if (SaveHasProfile()) {
        if (SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
            self->state = 8;
            self->substate = 0;
        }
    }
}
