// bdc 0x089d0768 NetCharaPrepareHeader
#include "bdc.h"

/* Refreshes the sync fields of the character's outgoing header before sending: `seq` = last local
   sequence `localSeq`, `readyFrames` = `readyFrames`, `ack` = peer's `inHdr.lastSeq`, `lastSeq` =
   `frameCounterOut`, and when the outgoing ring holds records (`outCount`) `firstSeq` = first
   record's sequence and `count` = record count (also tracked in `pendingRecords`/`lastOutSeq`). */
void NetCharaPrepareHeader(NetChara *self)
{
    u32 *first;
    u32 *rec;
    u32 i;

    CoreLockAcquire(self->lock);
    first = (u32 *)self->outBuf;
    self->outHdr.seq = self->localSeq;
    self->outHdr.readyFrames = (u8)self->readyFrames;
    self->outHdr.ack = self->inHdr.lastSeq;
    self->outHdr.firstSeq = 0;
    self->outHdr.lastSeq = self->frameCounterOut;
    self->outHdr.count = 0;
    self->outHdr.pad11 = 0;
    self->pendingRecords = 0;
    if (self->outCount != 0) {
        rec = first;
        for (i = 0; i < self->outCount; i++) {
            self->pendingRecords++;
            self->lastOutSeq = *rec;
            rec += 10;
        }
        self->outHdr.firstSeq = *first;
        self->outHdr.count = (s8)self->pendingRecords;
    }
    CoreLockRelease(self->lock);
}
