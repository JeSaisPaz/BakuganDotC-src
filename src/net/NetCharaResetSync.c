// bdc 0x089d049c NetCharaResetSync
#include "bdc.h"

/* Resets a net character's lock-step counters under its lock: clears the push latch `+0x120`, ready
   byte `+5`, sequence/frame counters `+0x124..+0x150`, sets the read cursors `+0x154`/`+0x158` to
   -1 and the countdown `+0x11c` to 0, and clears the lobby flag word (`NetCharaClearLobbyFlag`).
    */

void NetCharaResetSync(NetChara *self)

{
  CoreLockAcquire(self->lock);
  self->msgPending = '\0';
  self->ready = '\0';
  self->nextMsgSeq = 0;
  self->readCount = 0;
  self->peerSeq = 0;
  self->lastOutSeq = 0;
  self->frameCounterOut = 0;
  self->frameCounter = 0;
  self->localSeq = 0;
  self->outCount = 0;
  self->pendingRecords = 0;
  self->recvAckCount = 0;
  self->slotCount = 0;
  self->readyFrames = 0;
  self->firstReadSeq = -1;
  self->lastCommitSeq = -1;
  self->syncCountdown = 0;
  NetCharaClearLobbyFlag(self);
  CoreLockRelease(self->lock);
  return;
}

