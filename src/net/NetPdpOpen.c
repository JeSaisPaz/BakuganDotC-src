// bdc 0x089d3130 NetPdpOpen
#include "bdc.h"

/* Opens the two ad-hoc PDP sockets on the WLAN MAC (`sceNetAdhocPdpCreate` on ports 100 and 200,
   0x2000-byte buffers) if not yet open. When both are open (or the MAC could not be read: the
   success flag stays 1), timestamps the holder's `lastRecvTime`, zeroes `silenceSecs`, sets
   `shortTimedOut`, creates the local net character for the own MAC (`NetCharaFindOrCreate`,
   `CONetPDP.cpp` line 0x99), marks it local, copies the player name (`SaveProfileGetPlayerName`
   → `outHdr.hostInfo`) and sets/clears `outHdr.lenFlags` bit `0x1000` per `NetAdhocIsHost`.
   Always clears `timedOut` and `packetReceived`. Returns 1 on success, 0 if a socket could not be
   created (the character creation result does not affect the return value). */

int NetPdpOpen(NetPdp *self)
{
    int ok;
    int id;
    NetChara *chara;
    u8 mac[8];

    ok = 1;
    if (sceWlanGetEtherAddr(mac) == 0) {
        if (self->recvId == -1) {
            id = sceNetAdhocPdpCreate(mac, 100, 0x2000, 0);
            if (id > 0) {
                self->recvId = id;
            } else {
                ok = 0;
            }
        }
        if (self->sendId == -1) {
            id = sceNetAdhocPdpCreate(mac, 200, 0x2000, 0);
            if (id > 0) {
                self->sendId = id;
            } else {
                ok = 0;
            }
        }
    }
    if (ok != 0) {
        sceRtcGetCurrentClockLocalTime(&g_netPdpState->lastRecvTime);
        g_netPdpState->silenceSecs = 0;
        self->shortTimedOut = 1;
        chara = (NetChara *)NetCharaFindOrCreate(mac);
        if (chara != NULL) {
            CoreLockAcquire(chara->lock);
            chara->isLocal = 1;
            if (SaveHasProfile()) {
                SaveProfileGetPlayerName((SaveProfile *)SaveGetProfile(),
                                         (char *)chara->outHdr.hostInfo);
            }
            if (NetAdhocHasManager()) {
                if (NetAdhocIsHost((NetAdhocConn *)NetAdhocGetManager())) {
                    chara->outHdr.lenFlags = (s16)(chara->outHdr.lenFlags | 0x1000);
                } else {
                    chara->outHdr.lenFlags = (s16)(chara->outHdr.lenFlags & ~0x1000);
                }
            }
            CoreLockRelease(chara->lock);
        }
    }
    self->timedOut = 0;
    self->packetReceived = 0;
    return ok;
}
