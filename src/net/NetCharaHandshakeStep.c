// bdc 0x089d1924 NetCharaHandshakeStep
#include "bdc.h"

/* Lobby handshake state machine of one connected remote net character, run by `NetCharaUpdate`
   while the adhoc mode is 1. It reads the peer's four status bytes for us from its receive buffer
   (`NetCharaFindOwnRecvEntry` → `inHdr.handshake[0..3]`, word → `inHdr.flags`), then advances the
   handshake state `outHdr.handshake[1]` on the host side (`NetAdhocIsHost`) or the client side;
   the sub-state `outHdr.handshake[0]` and `outHdr.seq` are written back unchanged.
   Host: when the peer asks to join (peer state 1) and fewer than two peers are registered, it adds
   the peer's `NetPlayPeer` record (info `inHdr.hostInfo`, MAC `inHdr.mac`) to the NetPlay peer
   table (`NetPlayAddPeer`; state 2, else 3) and removes it again when the peer leaves
   (`NetPlayRemovePeer`; peer state 5 → state 0).
   Client: when the peer is the selected host (`NetPlayGetSelectedHost`, `NetCharaFindByMac`) it
   enters state 1; on the host's accept it copies the host MAC into the local character
   (`NetCharaGetByIndex(0)->outHdr.joinedHostMac`) and enters NetPlay state 5 (join wait); in state 4
   it takes over the host's member list (`NetPlaySetPeers`, `peerBlock`, `inHdr.peerCount`) and
   follows it into NetPlay state 6 (join launch), or clears the pairing and falls back to NetPlay
   state 4 (scan). */

void NetCharaHandshakeStep(NetChara *self)
{
    u8 sub;
    u8 state;
    u32 seq;
    u32 status0;
    u32 status1;
    u32 status2;
    u32 status3;
    u32 word8;
    u32 flags;
    s32 peerState;
    bool isHost;
    s32 i;
    NetPlayPeer peer;
    NetPlayPeer *host;
    NetChara *local;
    s32 netState;

    sub = self->outHdr.handshake[0];
    state = self->outHdr.handshake[1];
    seq = self->outHdr.seq;
    status0 = 0xffffffff;
    status1 = 0xffffffff;
    status2 = 0xffffffff;
    status3 = 0xffffffff;
    word8 = 0xffffffff;
    flags = 0;
    isHost = false;
    NetCharaFindOwnRecvEntry(self, &status0, &status1, &status2, &status3, &word8, &flags);
    self->inHdr.handshake[0] = (u8)status0;
    self->inHdr.handshake[1] = (u8)status1;
    self->inHdr.handshake[2] = (u8)status2;
    self->inHdr.handshake[3] = (u8)status3;
    self->inHdr.flags = flags;
    peerState = (s32)status1;

    if (NetAdhocHasManager()) {
        if (NetAdhocIsHost((NetAdhocConn *)NetAdhocGetManager())) {
            isHost = true;
        }
    }

    if (isHost) {
        switch (state) {
        case 0:
            /* Peer asks to join. */
            if (peerState != 1) {
                break;
            }
            state = 3;
            if (!NetPlayHasManager()) {
                break;
            }
            if (NetPlayGetPeerCount((NetPlay *)NetPlayGetManager()) >= 2) {
                break;
            }
            memset(&peer, 0, sizeof(peer));
            for (i = 0; i < 0x18; i++) {
                peer.info[i] = self->inHdr.hostInfo[i];
            }
            peer.info[0x18] = self->inHdr.hostInfoEnd;
            memcpy(peer.mac, self->inHdr.mac, 6);
            if (NetPlayAddPeer((NetPlay *)NetPlayGetManager(), (const u8 *)&peer) != 0) {
                state = 2;
            }
            break;
        case 2:
            if (peerState == 4) {
                state = 4;
            } else if (peerState == 5) {
                memset(&peer, 0, sizeof(peer));
                for (i = 0; i < 0x18; i++) {
                    peer.info[i] = self->inHdr.hostInfo[i];
                }
                peer.info[0x18] = self->inHdr.hostInfoEnd;
                memcpy(peer.mac, self->inHdr.mac, 6);
                NetPlayRemovePeer((NetPlay *)NetPlayGetManager(), (const u8 *)&peer);
                state = 0;
            }
            break;
        case 3:
            if (peerState == 5) {
                state = 0;
            }
            break;
        case 4:
            if (peerState == 5) {
                memset(&peer, 0, sizeof(peer));
                for (i = 0; i < 0x18; i++) {
                    peer.info[i] = self->inHdr.hostInfo[i];
                }
                peer.info[0x18] = self->inHdr.hostInfoEnd;
                memcpy(peer.mac, self->inHdr.mac, 6);
                NetPlayRemovePeer((NetPlay *)NetPlayGetManager(), (const u8 *)&peer);
                state = 0;
            } else if (NetPlayHasManager()) {
                netState = NetPlayGetState((NetPlay *)NetPlayGetManager());
                if (netState == 3 || netState == 6) {
                    state = 6;
                }
            }
            break;
        case 5:
            if (peerState == 0) {
                state = 0;
            }
            break;
        case 6:
            if (peerState == 5) {
                memset(&peer, 0, sizeof(peer));
                for (i = 0; i < 0x18; i++) {
                    peer.info[i] = self->inHdr.hostInfo[i];
                }
                peer.info[0x18] = self->inHdr.hostInfoEnd;
                memcpy(peer.mac, self->inHdr.mac, 6);
                NetPlayRemovePeer((NetPlay *)NetPlayGetManager(), (const u8 *)&peer);
                state = 0;
            }
            break;
        default:
            /* 1, 7 and out-of-range states stay. */
            break;
        }
    } else {
        switch (state) {
        case 0:
            /* Is this peer the host we selected? */
            if (!NetPlayHasManager()) {
                break;
            }
            host = (NetPlayPeer *)NetPlayGetSelectedHost((NetPlay *)NetPlayGetManager());
            if (host == NULL) {
                break;
            }
            if (NetCharaFindByMac(host->mac) == self) {
                state = 1;
            }
            break;
        case 1:
            if (peerState == 3) {
                state = 5;
            } else if (peerState == 2) {
                /* Host accepted us. */
                state = 5;
                host = (NetPlayPeer *)NetPlayGetSelectedHost((NetPlay *)NetPlayGetManager());
                local = NetCharaGetByIndex(0);
                if (host != NULL && local != NULL) {
                    state = 4;
                    memcpy(local->outHdr.joinedHostMac, host->mac, 6);
                    NetPlaySetState((NetPlay *)NetPlayGetManager(), 5);
                }
            }
            break;
        case 4:
            host = (NetPlayPeer *)NetPlayGetSelectedHost((NetPlay *)NetPlayGetManager());
            if (host == NULL) {
                local = NetCharaGetByIndex(0);
                if (local != NULL) {
                    memset(local->outHdr.joinedHostMac, 0, 6);
                    state = 5;
                }
            } else if (NetCharaFindByMac(host->mac) == NULL) {
                /* Host vanished: drop the pairing and go back to scanning. */
                local = NetCharaGetByIndex(0);
                if (local != NULL) {
                    memset(local->outHdr.joinedHostMac, 0, 6);
                    NetPlaySetState((NetPlay *)NetPlayGetManager(), 4);
                    state = 0;
                }
            } else {
                if (NetPlayHasManager()) {
                    if (NetPlayGetState((NetPlay *)NetPlayGetManager()) == 5) {
                        NetPlaySetPeers((NetPlay *)NetPlayGetManager(), self->peerBlock,
                                        self->inHdr.peerCount);
                    }
                }
                if (peerState == 6) {
                    state = 6;
                    NetPlaySetState((NetPlay *)NetPlayGetManager(), 6);
                }
            }
            break;
        case 5:
            if (peerState == 0) {
                state = 0;
            }
            break;
        default:
            /* 2, 3, 6, 7 and out-of-range states stay. */
            break;
        }
    }

    self->outHdr.handshake[0] = sub;
    self->outHdr.handshake[1] = state;
    self->outHdr.seq = seq;
}
