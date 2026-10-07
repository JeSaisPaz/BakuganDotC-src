// bdc 0x089d2ff8 NetPdpStep
#include "bdc.h"

/* State machine of the `CONetPDP` object, run every network frame: state 1 opens the sockets
   (`NetPdpOpen`; on success, once the local character exists, timestamps its `stopwatch` and
   goes to 2; on failure goes to 3 once the adhoc manager is gone or stopped), 2 runs
   `NetCharaMgrUpdate` and `NetPdpUpdate`, 3 clears `packetReceived`, deletes the sockets
   (`NetPdpDeleteSockets`) and goes to 4 when both are gone, 4 destroys the holder
   (`NetPdpDestroy`), state 0 restarts at 1; states >= 5 do nothing. */

void NetPdpStep(NetPdp *self)
{
    NetChara *chara;
    ScePspDateTime now;

    switch (self->state) {
    case 1:
        if (NetPdpOpen(self) != 0) {
            chara = NetCharaGetByIndex(0);
            if (chara != NULL) {
                sceRtcGetCurrentClockLocalTime(&now);
                chara->stopwatch = now;
                self->state = 2;
            }
        } else if (!NetAdhocHasManager()) {
            self->state = 3;
        } else if (NetAdhocIsStopped((NetAdhocConn *)NetAdhocGetManager())) {
            self->state = 3;
        }
        break;
    case 2:
        NetCharaMgrUpdate();
        NetPdpUpdate(self);
        break;
    case 3:
        self->packetReceived = 0;
        if (NetPdpDeleteSockets(self)) {
            self->state = 4;
        }
        break;
    case 4:
        NetPdpDestroy();
        break;
    case 0:
        self->state = 1;
        break;
    default:
        break;
    }
}
