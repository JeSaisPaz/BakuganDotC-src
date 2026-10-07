// bdc 0x089d0164 NetCharaCtor
#include "bdc.h"

/* Constructor of a `CONetChara` net-character object: under the `g_netCharaMgr` lock it inserts
   `self` into the manager list (`NetCharaListInsert(list, self, 1000)`), copies the 6-byte peer MAC
   `mac` (when given) to `outHdr.mac` and clears the two handshake bytes `outHdr.handshake[0..1]`, and
   clears the whole received header `inHdr`; then creates its own `CoreLock` `"CONetChara"`
   (LwMutex, `lock`; NULL if the allocation failed), allocates and zeroes the two 0x800-byte message
   buffers (outgoing `outBuf`, received `recvBuf`), sets `connected`, zeroes the stopwatch, the
   sequence/frame counters and the state bytes, and finishes with `NetCharaClearLobbyFlag`.
   Returns `self`. */

NetChara *NetCharaCtor(NetChara *self, const u8 *mac)
{
    bool fromLow;
    CoreLock *lock;
    u8 *buf;
    s32 i;

    CoreLockAcquire(g_netCharaMgr->lock);
    NetCharaListInsert((CoreList *)g_netCharaMgr->list, self, 1000);
    if (mac != NULL) {
        for (i = 0; i < 6; i++) {
            self->outHdr.mac[i] = mac[i];
        }
        self->outHdr.handshake[0] = 0;
        self->outHdr.handshake[1] = 0;
    }
    memset(&self->inHdr, 0, sizeof(NetCharaPacketHeader));
    CoreLockRelease(g_netCharaMgr->lock);

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    lock = MemAlloc(0x38, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (lock != NULL) {
        CoreLockInit(lock, "CONetChara", CORE_LOCK_LWMUTEX);
    }
    self->lock = lock;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    buf = MemAlloc(0x800, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->outBuf = buf;
    memset(buf, 0, 0x800);
    self->outLen = 0;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    buf = MemAlloc(0x800, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->recvBuf = buf;
    memset(buf, 0, 0x800);
    self->recvLen = 0;

    self->connected = 1;
    self->hasRole = 0;
    self->matched = 0;
    memset(&self->stopwatch, 0, sizeof(ScePspDateTime));
    self->id = 0;
    self->recreateOnDrop = 0;
    self->isLocal = 0;
    self->msgPending = 0;
    self->ready = 0;
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
    self->secsSincePacket = 0;
    self->syncCountdown = 0;
    NetCharaClearLobbyFlag(self);
    return self;
}
