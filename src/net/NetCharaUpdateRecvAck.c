// bdc 0x089d0820 NetCharaUpdateRecvAck
#include "bdc.h"

/* After a client receives a header without a peer block (flag `0x800` at `+0x72`): stores the
   record count (byte `+0x50`) in `+0x148` and `min(+0x48, +0x44)` (the peer's acknowledged/last
   sequence words) in `+0x12c`, under the character lock. */

void NetCharaUpdateRecvAck(NetChara *self)

{
  u32 other;
  u32 ack;
  int i;

  CoreLockAcquire(self->lock);
  self->recvAckCount = 0;
  if (((self->inHdr).lenFlags & 0x800) != 0) {
    for (i = 0; i < (self->inHdr).count; i++) {
      self->recvAckCount = self->recvAckCount + 1;
    }
    other = (self->inHdr).lastSeq;
    ack = (self->inHdr).ack;
    if (other < ack) {
      ack = other;
    }
    self->peerSeq = ack;
  }
  CoreLockRelease(self->lock);
  return;
}
