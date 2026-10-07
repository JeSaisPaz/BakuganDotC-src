// bdc 0x089d332c NetPdpUpdate
#include "bdc.h"

/* Per-frame receive/send step of the `CONetPDP` socket object (from the net update `NetPdpStep`):
   drains `sceNetAdhocPdpRecv` on `recvId` into the 0x2000-byte `buffer` until a receive fails;
   packets from port 200 of an unknown or remote character are counted (`g_netPdpRecvCount`),
   checksummed (`adler32` over the payload vs. the leading word) and handed to
   `NetCharaReceivePacket`, or counted in `g_netPdpBadChecksumCount` on a mismatch. Any such
   packet refreshes `lastRecvTime` of `g_netPdpState`, clears the timeout bytes and sets
   `packetReceived`; otherwise `shortTimedOut` is set after 5 s and `timedOut` after 15 s of
   silence (both clear `packetReceived`), and
   `silenceSecs` follows the elapsed seconds. Then, when `sendId` is open, builds the outgoing
   packet (`NetCharaBuildPacket`) behind its checksum and, if the net heap is under 80 % used,
   sends it with `sceNetAdhocPdpSend` to port 100 of the peer (host MAC for role 2, client MAC
   otherwise, broadcast without a mode-2 manager), stamping `lastSendTime` on success. */

void NetPdpUpdate(NetPdp *self)
{
  bool gotPacket;
  int ret;
  int i;
  int secs;
  int us;
  u8 *payload;
  u8 *src;
  NetChara *chara;
  NetAdhocConn *conn;
  NetPdpState *state;
  u8 mac[6];
  u16 port;
  int len;

  len = 0;
  gotPacket = false;
  if (self->recvId > 0) {
    do {
      len = 0x2000;
      ret = sceNetAdhocPdpRecv(self->recvId, mac, &port, self->buffer, &len, 0, 1);
      if (port == 200) {
        chara = NetCharaFindByMac(mac);
        if (chara == NULL || !chara->isLocal) {
          if (ret != 0) {
            break;
          }
          g_netPdpRecvCount++;
          sceKernelDcacheWritebackInvalidateRange(self->buffer, len);
          payload = self->buffer + 4;
          len -= 4;
          if (*(u32 *)self->buffer == adler32(0, payload, len)) {
            NetCharaReceivePacket(payload, len);
          }
          else {
            g_netPdpBadChecksumCount++;
          }
          gotPacket = true;
        }
      }
    } while (ret == 0);
  }

  if (gotPacket) {
    sceRtcGetCurrentClockLocalTime(&g_netPdpState->lastRecvTime);
    self->timedOut = 0;
    self->shortTimedOut = 0;
    self->packetReceived = 1;
  }
  else {
    us = CoreRtcDiffMicroseconds(&g_netPdpState->lastRecvTime, NULL);
    if (us > 15000000) {
      self->timedOut = 1;
      self->packetReceived = 0;
    }
    else if (us > 5000000) {
      self->shortTimedOut = 1;
      self->packetReceived = 0;
    }
    secs = us / 1000000;
    state = g_netPdpState;
    if (state->silenceSecs < secs) {
      state->silenceSecs = secs;
    }
    else if (secs + 1 < state->silenceSecs) {
      state->silenceSecs = secs;
    }
  }

  if (self->sendId <= 0) {
    return;
  }
  payload = self->buffer + 4;
  len = NetCharaBuildPacket(payload);
  sceKernelDcacheWritebackInvalidateRange(payload, len);
  *(u32 *)self->buffer = adler32(0, payload, len);
  len += 4;
  if (NetGetMallocUsedPercent() >= 80 || len <= 0) {
    return;
  }
  for (i = 0; i < 6; i++) {
    mac[i] = 0xff;
  }
  if (NetAdhocHasManager()) {
    conn = NetAdhocGetManager();
    if (conn->mode == 2) {
      conn = NetAdhocGetManager();
      if (conn->role == 2) {
        src = NetAdhocGetHostMac(NetAdhocGetManager());
      }
      else {
        src = NetAdhocGetClientMac(NetAdhocGetManager());
      }
      for (i = 0; i < 6; i++) {
        mac[i] = src[i];
      }
    }
  }
  port = 100;
  if (sceNetAdhocPdpSend(self->sendId, mac, port, self->buffer, len, 0, 1) == 0) {
    sceRtcGetCurrentClockLocalTime(&g_netPdpState->lastSendTime);
  }
}
