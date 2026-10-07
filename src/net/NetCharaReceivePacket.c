// bdc 0x089cfd9c NetCharaReceivePacket
#include "bdc.h"

/* Handles one received PDP packet of `len` bytes (ignored when `g_netCharaMgr` is missing, `len`
   is not positive or `packet` is NULL). Under the manager lock it finds the sender by the MAC in the
   packet header (`NetCharaFindByMac`); in adhoc modes 0/1 an unknown sender is created
   (`NetCharaFindOrCreate`), modes other than 0..2 drop the packet. A sender still missing is
   created (again) and timestamped, and the PDP timestamp is refreshed (`NetPdpTouch`). For a known
   sender it timestamps `stopwatch`, then (as a client, mode 2, into the local character instead)
   copies the header to `inHdr`, the optional 0x7c-byte peer block to `peerBlock`, the payload to
   `recvBuf` (length `recvLen` = `lenFlags & 0x7ff`) and, in mode 2, updates the ack count
   (`NetCharaUpdateRecvAck`). */

void NetCharaReceivePacket(void *packet, int len)
{
  NetCharaPacketHeader *hdr;
  NetChara *chara;
  NetAdhocConn *conn;
  u8 *payload;
  u32 n;
  int mode;

  if (g_netCharaMgr == NULL) {
    return;
  }
  hdr = (NetCharaPacketHeader *)packet;
  mode = 0;
  if (NetAdhocHasManager()) {
    conn = (NetAdhocConn *)NetAdhocGetManager();
    mode = conn->mode;
  }
  if (len <= 0 || hdr == NULL) {
    return;
  }
  CoreLockAcquire(g_netCharaMgr->lock);
  chara = NetCharaFindByMac(hdr->mac);
  if (mode < 2) {
    if (mode < 0) {
      goto unlock;
    }
    if (chara == NULL) {
      chara = (NetChara *)NetCharaFindOrCreate(hdr->mac);
    }
  } else if (mode > 2) {
    goto unlock;
  }
  if (chara == NULL) {
    chara = (NetChara *)NetCharaFindOrCreate(hdr->mac);
    sceRtcGetCurrentClockLocalTime(&chara->stopwatch); /* no NULL check, as compiled */
    NetPdpTouch((NetPdp *)NetPdpGet());
  } else {
    CoreLockAcquire(chara->lock);
    sceRtcGetCurrentClockLocalTime(&chara->stopwatch);
    if (mode == 2) {
      CoreLockRelease(chara->lock);
      chara = NetCharaGetByIndex(0);
      CoreLockAcquire(chara->lock);
    }
    memcpy(&chara->inHdr, hdr, sizeof(NetCharaPacketHeader));
    payload = (u8 *)(hdr + 1);
    if ((chara->inHdr.lenFlags & 0x800) == 0) {
      memcpy(chara->peerBlock, payload, sizeof(chara->peerBlock));
      payload += sizeof(chara->peerBlock);
    }
    n = chara->inHdr.lenFlags & 0x7ff;
    chara->recvLen = n;
    memcpy(chara->recvBuf, payload, n);
    if (mode == 2) {
      NetCharaUpdateRecvAck(chara);
    }
    CoreLockRelease(chara->lock);
  }
unlock:
  CoreLockRelease(g_netCharaMgr->lock);
}
