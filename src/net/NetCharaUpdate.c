// bdc 0x089d0ea0 NetCharaUpdate
#include "bdc.h"

/* Per-frame update of one net character (from `NetCharaMgrUpdate`), under its lock. For a remote,
   connected character (`isLocal` clear, `connected` set) it checks liveness:
   - not yet `matched`: `sceNetAdhocctlGetPeerInfo` on its MAC; the peer timed out when its last
     beacon is older than `g_netPeerTimeoutUs` (10 s), and error 0x80410716 clears `connected`
     directly; then the receive stopwatch (`CoreRtcDiffMicroseconds`) > 10 s also times it out;
   - `matched`: only the stopwatch > 10 s times it out, and `secsSincePacket` follows the
     stopwatch in whole seconds (raised at once, lowered only when more than one second off).
   A timed-out peer is marked disconnected and removed from the pending invites of `g_netInvite`
   (`NetInviteIsPendingA`/`NetInviteClearPendingA`, `NetInviteIsPendingB`/`NetInviteClearPendingB`).
   Every still-connected character (local ones too) then runs `NetCharaHandshakeStep` while the
   adhoc connection's `linkedRole` is 1. */

void NetCharaUpdate(NetChara *self)
{
  bool timedOut;
  int ret;
  int elapsed;
  int secs;
  u8 *mac;
  NetAdhocConn *conn;
  SceNetAdhocctlPeerInfo info;

  CoreLockAcquire(self->lock);
  if (!self->isLocal && self->connected) {
    timedOut = false;
    if (!self->matched) {
      ret = sceNetAdhocctlGetPeerInfo(self->outHdr.mac, 0x98, &info);
      if (ret == 0) {
        if ((u64)(sceKernelGetSystemTimeWide() - info.timestamp) > g_netPeerTimeoutUs) {
          timedOut = true;
        }
      }
      else if (ret == (int)0x80410716) {
        self->connected = 0;
      }
      if (CoreRtcDiffMicroseconds(&self->stopwatch, NULL) > 10000000) {
        timedOut = true;
      }
    }
    else {
      elapsed = CoreRtcDiffMicroseconds(&self->stopwatch, NULL);
      if (elapsed > 10000000) {
        timedOut = true;
      }
      secs = elapsed / 1000000;
      if (self->secsSincePacket < secs) {
        self->secsSincePacket = secs;
      }
      else if (secs + 1 < self->secsSincePacket) {
        self->secsSincePacket = secs;
      }
    }
    if (timedOut) {
      self->connected = 0;
      if (NetHasInvite()) {
        mac = self->outHdr.mac;
        if (NetInviteIsPendingA(NetInviteGet(), mac) != 0) {
          NetInviteClearPendingA(NetInviteGet());
        }
        if (NetInviteIsPendingB(NetInviteGet(), mac) != 0) {
          NetInviteClearPendingB(NetInviteGet());
        }
      }
    }
  }
  if (self->connected) {
    conn = (NetAdhocConn *)NetAdhocGetManager();
    if (conn->linkedRole < 2 && conn->linkedRole > 0) {
      NetCharaHandshakeStep(self);
    }
  }
  CoreLockRelease(self->lock);
}
